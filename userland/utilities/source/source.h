namespace MyNamespace {

int function();

#define USE_VAR1

#define INT_Z int z

struct StructHeader {
    int var1;
#ifndef USE_VAR1
    int var1;
#else
    int var3;
#endif
};

}