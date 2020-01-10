Name:           libHBAAPI
Version:        2.2
Release:        1%{?dist}
Summary:        SNIA HBAAPI library

Group:          System Environment/Libraries
License:        SNIA
URL:            http://www.sourceforge.net/projects/hbaapi
Source0:        hbaapi_src_2.2.tgz
Source1:        hbaapi_build_2.2.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

# BuildRequires:  
# Requires:       

%description
The SNIA HBA API library

%package        devel
Summary:        Development files for %{name}
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release} pkgconfig

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.


%prep
%setup -q -n hbaapi_src_2.2
%setup -q -T -D -a 1 -n hbaapi_src_2.2


%build
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
%doc readme.txt
%config(noreplace) %{_sysconfdir}/hba.conf
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root,-)
%{_includedir}/*
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc


%changelog
* Tue Feb 24 2009 Chris Leech <christopher.leech@intel.com> - 2.2-1
- initial packaging of hbaapi 2.2

