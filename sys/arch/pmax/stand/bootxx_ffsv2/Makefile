# $NetBSD: Makefile,v 1.1 2009/04/21 16:08:39 tsutsui Exp $

PRIMARY_PROG=	bootxx_ffsv2
CPPFLAGS+=	-DLIBSA_SINGLE_FILESYSTEM=ffsv2 \
		-DBOOTXX_FS_NAME='"FFSv2"'
# Use small daddr_t to avoid code bloat
CPPFLAGS+=	-D__daddr_t=int32_t

PRIMARY_MAX_LOAD!=	expr 8192 - 512

.include "../Makefile.booters"
