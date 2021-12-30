__extension__ typedef int guard_t __attribute__((mode(__DI__)));
 
extern "C" int __cxa_guard_acquire(guard_t*);
extern "C" void __cxa_guard_release(guard_t*);
extern "C" void __cxa_guard_abort(guard_t*);
 
extern "C" int __cxa_guard_acquire(guard_t* g) 
{
	return !*(char *)(g);
}
 
extern "C" void __cxa_guard_release(guard_t* g)
{
	*(char *)g = 1;
}
 
extern "C" void __cxa_guard_abort(guard_t* g)
{

}
