# $NetBSD: mkvars.mk,v 1.3 2010/03/22 12:03:04 cegger Exp $

MKEXTRAVARS= \
	MACHINE \
	MACHINE_ARCH \
	MACHINE_CPU \
	HAVE_BINUTILS \
	HAVE_GCC \
	HAVE_GDB \
	OBJECT_FMT \
	TOOLCHAIN_MISSING \
	EXTSRCS \
	MKMANZ \
	MKBFD \
	MKCOMPAT \
	MKDYNAMICROOT \
	MKMANPAGES \
	MKXORG \
	X11FLAVOR \
	USE_INET6 \
	USE_KERBEROS \
	USE_LDAP \
	USE_YP \
	NETBSDSRCDIR \
	MAKEVERBOSE

#####

.include <bsd.own.mk>

.if (${MKMAN} == "no" || empty(MANINSTALL:Mmaninstall))
MKMANPAGES=no
.else
MKMANPAGES=yes
.endif

.if ${MKX11} != "no"
. if ${X11FLAVOUR} == "Xorg"
MKXORG:=yes
MKX11:=no
. else
MKXORG:=no
. endif
.endif

#####

mkvars: mkvarsyesno mkextravars mksolaris .PHONY

mkvarsyesno: .PHONY
.for i in ${_MKVARS.yes}
	@echo $i="${$i}"
.endfor
.for i in ${_MKVARS.no}
	@echo $i="${$i}"
.endfor

mkextravars: .PHONY
.for i in ${MKEXTRAVARS}
	@echo $i="${$i}"
.endfor

mksolaris: .PHONY
.if (${MKDTRACE} != "no" || ${MKZFS} != "no")
	@echo MKSOLARIS="yes"
.else
	@echo MKSOLARIS="no"
.endif