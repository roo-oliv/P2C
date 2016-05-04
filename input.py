def foo(a, b):
    return a*b

def bar(a):
    for i in [1,2,3,4]:
        print(foo(a,i))

bar(7)
