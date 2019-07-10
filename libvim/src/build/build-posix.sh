unameOut="$(uname -s)"

case "${unameOut}" in
    Linux*) CFLAGS="CFLAGS=-fPIC"; LDFLAGS="LDFLAGS=-lacl" ;;
    *)      CFLAGS="" LDFLAGS="";;
esac

./configure --disable-selinux ${CFLAGS} ${LDFLAGS}
make installlibvim DESTDIR=$cur__install
