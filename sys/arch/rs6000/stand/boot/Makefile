#	$NetBSD: Makefile,v 1.4 2011/01/22 19:19:22 joerg Exp $

NOMAN= # defined

.include <bsd.own.mk>

COMMON= ${.CURDIR}/../common
COMMONOBJ!=	cd ${COMMON} && ${PRINTOBJDIR}

.PATH: ${.CURDIR}/../boot ${COMMONOBJ}

S= ${.CURDIR}/../../../..
L= ${COMMONOBJ}/lib

MKBOOTIMAGE= ${.CURDIR}/../../../powerpc/stand/mkbootimage

BASE?=		boot
PROG=		${BASE}
NEWVERSWHAT=	"BOOT"

SRCS= srt0.s
SRCS+= boot.c clock.c com.c conf.c cons.c devopen.c
SRCS+= filesystem.c inkernel.c io.c tgets.c prf.c monitor.c
SRCS+= ns16550.c vers.c
SRCS+= iplcb.c

CFLAGS= -Os -mmultiple -ffreestanding
CFLAGS+= -Werror -Wall -Wstrict-prototypes -Wmissing-prototypes
AFLAGS= -x assembler-with-cpp

CPPFLAGS= -nostdinc -I${.OBJDIR} -I${MKBOOTIMAGE} -I${S}
CPPFLAGS+= -D_STANDALONE -DRELOC=${RELOC} -DUSE_SCAN
CPPFLAGS+= -D__daddr_t=int32_t
#CPPFLAGS+= -DDBMONITOR
CPPFLAGS+= -DCONS_SERIAL -DCOMSPEED=9600 -DCOMPORT=0x30

STRIPFLAG=
BINMODE= 444

# XXX SHOULD NOT NEED TO DEFINE THESE!
LIBCRT0=
LIBC=
LIBCRTBEGIN=
LIBCRTEND=

RELOC= 0x800000
#RELOC= 0x0

CLEANFILES+= vers.c machine powerpc

LIBS= ${L}/sa/libsa.a ${L}/kern/libkern.a ${L}/z/libz.a

.if !make(obj) && !make(clean) && !make(cleandir)
.BEGIN: machine powerpc
.NOPATH: machine powerpc

machine::
	-rm -f $@
	ln -s ${S}/arch/${MACHINE}/include $@

powerpc::
	-rm -f $@
	ln -s ${S}/arch/powerpc/include $@
.endif

vers.c: ${.CURDIR}/../boot/version
	${HOST_SH} ${S}/conf/newvers_stand.sh ${${MKREPRO} == "yes" :?:-D} \
	    ${.CURDIR}/../boot/version "rs6000" ${NEWVERSWHAT}

${PROG}: ${OBJS} ${LIBS}
	${LD} -o ${PROG} -s -N -T ${.CURDIR}/../boot/ld.script \
		-Ttext ${RELOC} ${OBJS} ${LIBS}

.include <bsd.prog.mk>
