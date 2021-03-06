# The original name of the software package is 'xforms' but the
# rpm packages go by the name 'libforms' in all distros. We need
# the original name in some situations.

%define origname xforms

# Building a debug package doesn't seem to make too much sense

%define debug_package %{nil}


# Here we start...

Summary:         XForms library
Name:            libforms
Version:         1.2.5pre2
Release:         1%{?dist}
Source0:         %{name}-%{version}.tar.gz
License:         LGPLv2+
Group:           Development/Libraries

# To build the libraries we need the libXpm, libjpeg and libGL development
# packages (which in turn depend on the corresponding library packages).
# The package names seem to differ a bit between distributions, though...

BuildRequires:   libjpeg-devel

%if %{_host_vendor} == mageia
BuildRequires:   libxpm-devel
BuildRequires:   libmesagl1-devel
%else
BuildRequires:   libXpm-devel
BuildRequires:   mesa-libGL-devel
%endif

BuildRoot:       %{_tmppath}/%{name}-buildroot
Prefix:          %{_prefix}
URL:             http://xforms-toolkit.org/

%description
This is a GUI toolkit based on the X library for X Window Systems.
It features a rich set of objects, such as buttons, sliders, and
menus etc. integrated into an easy and efficient object/event
callback execution model that allows fast and easy construction
of X-applications. In addition, the library is extensible and new
objects can easily be created and added to the library.


# Beside the normal package create a "development" package, containing
# the header files for the three libraries, the static versions of the
# libraries and the fdesign and fd2ps programs.

%package devel
Summary:         Header files and tools
Group:           Development/Libraries
Requires:        %{name} = %{version}-%{release}

%description devel
Header files, tools and static libraries for development with XForms.


# Also create a documentation package with the full documentation in
# info, HTML and PDF format. This is an architecture-independent
# package.
#
# Note: older rpmbuild versions get very unhappy when you try to build
# a 'noarch' sub-package together with architecture-dependent packages,
# spitting out weird error messages and refusing to build the package,
# so use 'noarch' only with newer versions of rpmbuild.

%package doc
Summary:         XForms documentation
Group:           Development/Libraries
Requires(post):  info
Requires(preun): info
BuildRequires:   texi2html
BuildRequires:   texinfo
BuildRequires:   texinfo-tex
BuildRequires:   ImageMagick
%if 0%{?fedora} >= 10 || 0%{?rhel} >= 6 || 0%{?centos} >= 6 || 0%{?suse_version} >= 1230
BuildArch:       noarch
%endif

%description doc
Info, HTML, PDF documentation and demo programs for XForms.


# Copy and unpack the tar-ball

%prep
%setup -q


# Build everything ('make' can be done in parallel)

%build
./configure --prefix=%{_prefix} \
            --mandir=%{_mandir} \
            --bindir=%{_bindir} \
            --libdir=%{_libdir} \
            --infodir=%{_infodir} \
            --htmldir=%{_docdir}/%{name}/html \
            --pdfdir=%{_docdir}/%{name} \
            --enable-docs \
            --disable-demos \
            --disable-warnings \
            --disable-debug \
            --enable-optimization=-O2
make %{?_smp_mflags}
rm -rf demos/.deps


# Clear out the {buildroot} and install everything in {buildroot},
# then apply the following tweaks:
#  1) Delete the dir file from {_infodir}, it's not to be distributed
#     and gets created (or updated) during installation on the target
#     machine.
#  2) Strip the fdesign and fd2ps programs
#  3) Copy the directory with the demo programs to the documentation
#     directory
#  4) Create a symbolic link from the xforms.5 man page to libforms.5
#     (do that in a sub-process to avoid changing the working directory).

%install
rm -rf %{buildroot}
install -d -m 755 %{buildroot}
make DESTDIR=%{buildroot} install
rm -f %{buildroot}%{_infodir}/dir
/usr/bin/strip %{buildroot}%{_libdir}/*.so.*.*.*
/usr/bin/strip %{buildroot}%{_bindir}/*
cp -r %{_builddir}/%{name}-%{version}/demos %{buildroot}%{_docdir}/%{name}
`cd %{buildroot}%{_mandir}/man5 && ln -s %{origname}.5.gz %{name}.5.gz`


%clean
rm -rf %{buildroot}


# Set up list of files in the binary package: just the libraries and the
# man page for the libforms library.

%files
%{_libdir}/*.so.*
%{_mandir}/man5/*
%defattr(-,root,root)
%doc COPYING.LIB Copyright ChangeLog README


# Set up list of files going into the development package: header files,
# static libraries, utility programs and their man pages

%files devel
%{_bindir}/*
%{_includedir}/*
%{_libdir}/*.so
%{_libdir}/*.a
%{_libdir}/*.la
%{_mandir}/man1/*
%defattr(-,root,root)


# Set up list of files distributed with the documentation package:
# info, HTML, PDF documentation and demo programs

%files doc
%{_docdir}/%{name}/*
%{_infodir}/*
%defattr(-,root,root)


# Run ldconfig after install on the target machine to make the dynamic
# linker aware of the new libraries

%post
/sbin/ldconfig


# Run ldconfig after deinstall on the target machine of libraries

%postun
/sbin/ldconfig


# Update the info dir file and undo the compression that rpmbuild
# has applied to the image files below {_infodir} (these are image
# file for use by emacs' info mode and can't be read when gzip'ed).

%post doc
/sbin/install-info %{_infodir}/%{origname}.info %{_infodir}/dir || :
gunzip %{_infodir}/%{origname}_images/*.png.gz


# Before uninstalling remove the entry for the XForms info file from
# the dir file in {_infdir}. Also gzip the image files again because
# that's what they were during installation

%preun doc
if [ $1 = 0 ] ; then
  /sbin/install-info --delete %{_infodir}/%{origname}.info %{_infodir}/dir || :
  gzip %{_infodir}/%{origname}_images/*.png
fi


%changelog
* Sat Dec 21 2013  Jens Thoms Toerring <jt@toerring.de> 1.1.2.0-1
- spec file mostly rewritten for new 1.2.0 release, resulting
  packages now have a name starting with 'libforms'

* Thu Jun 1 2010 Jens Thoms Toerring <jt@toerring.de> 1.0.93sp1
- New release 1.0.93sp1

* Wed Nov 4 2009 Jens Thoms Toerring <jt@toerring.de> 1.0.93sp1
- New release 1.0.91sp2

* Sat Nov 22 2008 Jens Thoms Toerring <jt@toerring.de> 1.0.91
- New release 1.0.91 - lots of bug fixes

* Wed Oct 6 2004 Angus Leeming <angus.leeming@btopenworld.com> 1.0.90
- Re-write the 'post' and 'postun' scripts to create the
  symbolic links correctly without requiring the SO_VERSION hack.

* Fri May 7 2004 Angus Leeming <angus.leeming@btopenworld.com> 1.0.90
- add code to the 'post' script to modify libforms.la et al. to prevent
  libtool from complaining that the files have been moved.

* Thu May 6 2004 Angus Leeming <angus.leeming@btopenworld.com> 1.0.90
- fix 'Release' and 'Source0' info.
- add 'post' and 'postun' scripts to create and remove symbolic links,
  respectively.

* Thu May 6 2004 Angus Leeming <angus.leeming@btopenworld.com> 1.0.90
- no longer place devfiles and binfiles in ${RPM_BUILD_ROOT}.
  Prevents rpm from bombing with a "Checking for unpackaged files" error.

* Sat Aug 31 2002 Duncan Haldane <duncan_haldane@users.sourceforge.net> 1.0-RC4
- mv fdesign, fd2ps to devel.  restore xforms name of rpm.

* Sun Jul 14 2002 Greg Hosler <hosler@lugs.org.sg> 1.0-RC4
- Pass DESTDIR to makeinstall_std.

* Thu Jul 11 2002 Peter Galbraith <balbraith@dfo-mpo.gc.ca> 1.0-RC4
- Move from libxforms to libforms to match other distros.

* Tue Jul 8 2002 Chris Freeze <cfreeze@alumni.clemson.edu> 1.0-RC4
- First stab at spec file.
