if BUILD_MOTIF
XBUILD_SUB_MOTIF = mbedit mbnavedit mbvelocitytool
if BUILD_GL
XBUILD_SUB_GL = mbview mbgrdviz mbeditviz mbnavadjust
endif
endif

if BUILD_MBTRN
XBUILD_SUB_MBTRN = mbtrn
endif
if BUILD_MBTNAV
XBUILD_SUB_MBTNAV = mbtrnav
endif
if BUILD_MBTRNUTILS
XBUILD_SUB_MBTRNBIN = mbtrnutils
endif

if BUILD_OPENCVTOOLS
XBUILD_SUB_OPENCVTOOLS = photo
endif

if BUILD_QTTOOLS
XBUILD_SUB_QTTOOLS = qt-mbgrdviz-1 qt-mbgrdviz-2 qt-mbgui qt-mbgrdviz-3
endif

SUBDIRS =
if BUILD_GSF
  SUBDIRS += gsf
endif
SUBDIRS += surf
SUBDIRS += bsio
SUBDIRS += $(XBUILD_SUB_MBTRN)
SUBDIRS += $(XBUILD_SUB_MBTNAV)
SUBDIRS += mbio
SUBDIRS += mbaux
SUBDIRS += utilities
SUBDIRS += deprecated
SUBDIRS += $(XBUILD_SUB_MBTRNBIN)
SUBDIRS += gmt
SUBDIRS += otps
SUBDIRS += macros
SUBDIRS += share
SUBDIRS += man
SUBDIRS += html
SUBDIRS += ps
SUBDIRS += $(XBUILD_SUB_MOTIF)
SUBDIRS += $(XBUILD_SUB_GL)
SUBDIRS += $(XBUILD_SUB_OPENCVTOOLS)
SUBDIRS += $(XBUILD_SUB_QTTOOLS)

CLEANFILES =
DISTCLEANFILES =
