#define _JNI_IMPLEMENTATION_
#include <jni.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <sstream>
#include <iostream>
#include <list>
#include <vector>
#include <map>

#include <avian/system/system.h>
#include "avian/finder.h"
#include <avian/util/runtime-array.h>


#include "tail.h"

using namespace std;

#if (defined __MINGW32__)
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT __attribute__ ((visibility("default"))) \
  __attribute__ ((used))
#endif

static tail_t tail_self = NULL;

static vector<char*> tails_data;
static vector<unsigned> tails_length;

#ifdef __MINGW32__
	typedef std::wstring argstring;		// 2-byte UNICODE strings in Windows
#else
	typedef std::string argstring;		// UTF-8 encoded strings in POSIX systems
#endif

int maximumHeapSizeMegabytes;
std::map<std::string, std::string> definitions;
std::list<argstring> arguments;

extern "C"
{

	EXPORT const uint8_t* userClasses(unsigned* size)
	{
		if (size != NULL) *size = tails_length[1];
		return (const uint8_t*)tails_data[1];
	}
	EXPORT const uint8_t* bootClasses(unsigned* size)
	{
		if (size != NULL) *size = tails_length[2];
		return (const uint8_t*)tails_data[2];
	}

	JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {
		printf("Loading the program as an agent. This means you are possibly debugging...\n");
		return 0;
	}

	JNIEXPORT void JNICALL Agent_OnUnload(JavaVM *vm) {
		printf("Unloading agent\n");
	}

}

const char* mainClass()
{
  using namespace vm;

  System* system = makeSystem();

  class MyAllocator : public avian::util::Alloc {
   public:
    MyAllocator(System* s) : s(s)
    {
    }

    virtual void* allocate(size_t size)
    {
      void* p = s->tryAllocate(size);
      if (p == 0) {
        abort(s);
      }
      return p;
    }

    virtual void free(const void* p, size_t)
    {
      s->free(p);
    }

    System* s;
  } allocator(system);

  unsigned size;
  const uint8_t* jar = userClasses(&size);
  Finder* finder = makeFinder(system, &allocator, jar, (size_t)size);

  char* result = 0;

  System::Region* region = finder->find("META-INF/MANIFEST.MF");
  if (region) {
    size_t start = 0;
    size_t length;
    while (readLine(region->start(), region->length(), &start, &length)) {
      const unsigned PrefixLength = 12;
      if (strncasecmp("Main-Class: ",
                      reinterpret_cast<const char*>(region->start() + start),
                      PrefixLength) == 0) {
        result = static_cast<char*>(malloc(length + 1 - PrefixLength));
        memcpy(result,
               region->start() + start + PrefixLength,
               length - PrefixLength);
        result[length - PrefixLength] = 0;
        break;
      }
      start += length;
    }

    region->dispose();
  } else {
	  return NULL;
  }

  finder->dispose();

  system->dispose();

  return result;
}

int run()
{
	int maximumHeapSizeMegabytes = 16000;
	
	JavaVMInitArgs vmArgs;
	vmArgs.version = JNI_VERSION_1_2;
	vmArgs.nOptions = definitions.size() + 3;
	vmArgs.ignoreUnrecognized = JNI_TRUE;

	JavaVMOption options[vmArgs.nOptions];
	vmArgs.options = options;

	// Setting boot classpath
	options[0].optionString = const_cast<char*>("-Xbootclasspath:[bootClasses]");
	options[1].optionString = const_cast<char*>("-Xbootclasspath/a:[userClasses]");

	// Setting maximum heap memory amount
	stringstream xmxss;
	xmxss << "-Xmx" << maximumHeapSizeMegabytes << "m";
	options[2].optionString = const_cast<char*>(xmxss.str().c_str());	// "-Xmx16000m"

	// Adding definitions
	int i = 0;
	for (map<string, string>::const_iterator iter = definitions.begin(); iter != definitions.end(); iter++)
	{
		stringstream dss;
		dss << "-D" << (*iter).first << "=" << (*iter).second;
		char* tmp = new char[255];

		options[i + 3].optionString = new char[strlen(dss.str().c_str()) + 1];
		strcpy(options[i + 3].optionString, dss.str().c_str());
		i++;
	}

	JavaVM* vm;
	void* env;
	JNI_CreateJavaVM(&vm, &env, &vmArgs);
	JNIEnv* e = static_cast<JNIEnv*>(env);

	char* mainClassName = new char[tails_length[0] + 1];
	memcpy(mainClassName, tails_data[0], tails_length[0]);
	mainClassName[tails_length[0]] = 0;
	
	/*const char* mainClassName = mainClass();
	if (mainClassName == NULL) {
		printf("Fatal error: no main class specified\n");
	}*/
	
	jclass c = e->FindClass(mainClassName);
	if (not e->ExceptionCheck())
	{
		jmethodID m = e->GetStaticMethodID(c, "main", "([Ljava/lang/String;)V");
		if (not e->ExceptionCheck())
		{
			jclass stringClass = e->FindClass("java/lang/String");
			if (not e->ExceptionCheck())
			{
				jobjectArray a = e->NewObjectArray((jsize)(arguments.size()), stringClass, (jobject)0);
				if (not e->ExceptionCheck())
				{
					jsize index = 0;
					for (list<argstring>::const_iterator argi = arguments.begin(); argi != arguments.end(); argi++)
					{
#ifdef __MINGW32__
						// For Windows: Sending wide string to Java
						int arglen = (*argi).size();
						jstring arg = e->NewString((jchar*) ((*argi).c_str()), arglen);
#else
						// For other OS: Sending UTF8-encoded string to Java
						int arglen = (*argi).size();
						jstring arg = e->NewStringUTF((char*) ((*argi).c_str()));
#endif
						e->SetObjectArrayElement(a, index, arg);
						index++;
					}

					e->CallStaticVoidMethod(c, m, a);
				}
			}
		}
	}

	int exitCode = 0;
	if (e->ExceptionCheck())
	{
		exitCode = -1;
		e->ExceptionDescribe();
	}

	delete [] mainClassName;

	vm->DestroyJavaVM();

	return exitCode;
}

int main(int argc, char** argv) {
	int res;
	tail_t tail_self = tail_open(argv[0]);
	if (tail_self != NULL) {
		int i = 0;
		int can_read_more = 1;
		while (can_read_more) {
			size_t data_length = tail_get_length(tail_self);
			char* data = (char*)malloc(data_length);
			can_read_more = tail_read_previous(tail_self, data);

			tails_data.push_back(data);
			tails_length.push_back(data_length);
			i++;
		}

		tail_close(tail_self);
	
		res = run();
	} else {
		printf("Fatal error: Eeyore can't find his tail. He's very sad and disappointed\n");
		res = 1;
	}
	printf("\n");
	return res;
}