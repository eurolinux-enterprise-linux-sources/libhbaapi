Name:           libhbaapi
Version:        2.2.9
Release:        2%{?dist}
Summary:        SNIA HBAAPI library
Group:          System Environment/Libraries
License:        SNIA
URL:            http://open-fcoe.org
# This source was cloned from upstream git (libHBAAPI)
Source:         %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Patch0:         libhbaapi-2.2-9-dl-linking.patch
Patch1:         libhbaapi-2.2.9-portspeed.patch
BuildRequires:  automake libtool

%description
The SNIA HBA API library. C-level project to manage
Fibre Channel Host Bus Adapters.

%package        devel
Summary:        Development files for %{name}
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}
Requires:       pkgconfig

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.

%prep
%setup
%patch0 -p1 -b .ld-linking
%patch1 -p1 -b .portspeed

%build
./bootstrap.sh
%configure --disable-static
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%doc COPYING
%config(noreplace) %verify(not md5 size mtime) %{_sysconfdir}/hba.conf
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root,-)
%{_includedir}/*
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc

%changelog
* Thu Dec 17 2015 Chris Leech <cleech@redhat.com> - 2.2.9-2
- 1074125 add extended port speed definitions 

* Thu Jun 20 2013 Petr Šabata <contyk@redhat.com> - 2.2.9-1
- Update to 2.2.9, documentation and build scripts updates (#829815)

* Tue Oct 09 2012 Petr Šabata <contyk@redhat.com> - 2.2.6-1
- Switch to the Open-FCoE.org libHBAAPI fork (#862386)

* Tue Mar 27 2012 Petr Šabata <contyk@redhat.com> - 2.2-14
- Do not verify hba.conf size, mtime and checksum (#806731)

* Thu Feb 16 2012 Petr Šabata <contyk@redhat.com> - 2.2-13
- Update to hbaapi_build 2.2.5 (#788504)

* Mon Jul 25 2011 Petr Sabata <contyk@redhat.com> - 2.2-12
- Add SNIA license
- Related: rhbz#719585

* Thu Jul 14 2011 Petr Sabata <contyk@redhat.com> - 2.2-11
- Use hbaapi_build 2.2.4
- Resolves: rhbz#719585
- Related: rhbz#695941

* Mon Mar 08 2010 Jan Zeleny <jzeleny@redhat.com> - 2.2-10
- updated hbaapi_build to 2.2.2

* Wed Nov 04 2009 Jan Zeleny <jzeleny@redhat.com> - 2.2-9
- fixed linking with libdl

* Thu Jul 30 2009 Jan Zeleny <jzeleny@redhat.com> - 2.2-8
- added libtool to BuildRequires

* Thu Jul 30 2009 Jan Zeleny <jzeleny@redhat.com> - 2.2-7
- added automake to BuildRequires

* Thu Jul 30 2009 Jan Zeleny <jzeleny@redhat.com> - 2.2-6
- rebase of hbaapi_build code

* Fri Jul 24 2009 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 2.2-5
- Rebuilt for https://fedoraproject.org/wiki/Fedora_12_Mass_Rebuild

* Wed Apr 01 2009 Jan Zeleny <jzeleny@redhat.com> - 2.2-4
- added some info to description line
- replaced unoficial build source tarball with official one

* Tue Mar 31 2009 Jan Zeleny <jzeleny@redhat.com> - 2.2-3
- minor changes in spec file - filenames change, removal of
  duplicate patch files (included in build source tarball)
  
* Thu Mar 12 2009 Jan Zeleny <jzeleny@redhat.com> - 2.2-2
- correction of patches' names to correct format

* Tue Feb 24 2009 Chris Leech <christopher.leech@intel.com> - 2.2-1
- initial packaging of hbaapi 2.2

