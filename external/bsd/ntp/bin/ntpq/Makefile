#	$NetBSD: Makefile,v 1.5 2011/02/27 12:42:44 gdt Exp $

# NOGCCERROR=yes

.include <bsd.own.mk>

PROG=	ntpq
MAN=	ntpq.8
BINDIR=	/usr/sbin

SRCS=	ntpq.c					\
	ntpq-subs.c				\
	ntpq-opts.c

LDADD+=		-L${LIBNTP} -lntp -L${LIBOPTS} -lopts -L${LIBISC} -liscntp -ledit -lterminfo -lm 
#DPADD+=		${LIBISC}/libiscntp.a ${LIBOPTS}/libopts.a  ${LIBNTP}/libntp.a ${LIBEDIT} ${LIBTERMINFO} ${LIBM}

.include "${.CURDIR}/../Makefile.inc"

DIST=${IDIST}/ntpq

CPPFLAGS+=-I${IDIST}/include -I${IDIST}/libopts

.PATH: ${DIST}

ntpq.8:	ntpq.1
	${_MKTARGET_BUILD}
	${TOOL_SED} -e "s,NTPQ 1,NTPQ 8," $? > $@

CLEANFILES+=ntpq.8

.include <bsd.prog.mk>
