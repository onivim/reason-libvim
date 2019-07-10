unameOut="$(uname -s)"

# case "${unameOut}" in
#     Linux*) CFLAGS="CFLAGS=-fPIC";;
#     *)      CFLAGS="";;
# esac

# ./configure --disable-selinux ${CFLAGS}
./configure --disable-selinux "LDFLAGS=-lacl" 
make installlibvim DESTDIR=$cur__install
