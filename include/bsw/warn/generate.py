import yaml
import sys

def gcc (f, name, ver):
    f.write ("#if defined(WARN_PREDEF_COMPILER_GCC)\n");
    if ver:
        f.write ("#if WARN_PREDEF_COMPILER_GCC >= " + ver + "\n");
    f.write ("#pragma GCC diagnostic ignored \""+name+"\"\n");
    if ver:
        f.write ("#endif\n")
    f.write ("#endif\n")
        
def clang (f, name, ver):
    f.write ("#if defined(WARN_PREDEF_COMPILER_CLANG) || defined(__apple_build_version__)\n");
    if ver:
        f.write ("#if WARN_PREDEF_COMPILER_CLANG >= " + ver + "\n")
    f.write ("#pragma clang diagnostic ignored \""+name+"\"\n");
    if ver:
        f.write ("#endif\n")
    f.write ("#endif\n")
        
def msvc (f, name, ver):
    f.write ("#if defined(WARN_PREDEF_COMPILER_VISUALC)\n");
    if ver:
        f.write ("#if WARN_PREDEF_COMPILER_VISUALC >= " + ver + "\n");
    f.write ("#pragma warning (disable : "+str(name)+")\n");
    if ver:
        f.write ("#endif\n")
    f.write ("#endif\n")

if __name__ == "__main__":
    ifile = None
    if len(sys.argv) > 2:
        print ("USAGE: " + sys.argv[0] + " [input.yaml]");
        sys.exit (1);
    if len(sys.argv) == 2:
        ifile = sys.argv[1];
    else:
        ifile = "warns.yml"
    ds = None;
    with open(ifile, 'r') as stream:
        try:
            ds = yaml.load(stream)
        except yaml.YAMLError as exc:
            print(exc)
            sys.exit (1);
    for filename in ds.keys ():
        data = ds [filename];
        with open(filename, 'w') as f:
            for k in data.keys():
                functor = None
                entry = data [k]
                
                if "gcc" == k:
                    functor = gcc
                elif "msvc" == k:
                    functor = msvc
                elif "clang" == k:
                    functor = clang

                if functor:
                    material = None;
                    ver = None;

                    if not isinstance(entry, dict):
                        material = entry
                    else:
                        ver = str(entry ["ver"])
                        material = entry["warn"]
                    f.write ("\n")    
                    functor (f, material, ver);
                
