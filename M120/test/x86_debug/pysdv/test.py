def addspam(fn):
    def new(*args,**kargs):
        print("spam, spam, spam")
        if len(args)>0:
            print(args[0],args[1])
        if len(kargs)>0:
            print("a",kargs["a"],"b",kargs["b"])
        return fn(*args,**kargs)
    return new

@addspam
def useful(a, b):
    print( a**2 + b**2)


useful(a=3,b=4)