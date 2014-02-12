%define MODULEDIR "/usr/lib/elementary/modules/eail/linux-gnu-i686-1.7.99"
Name: eail-its
Summary: Test suite for the Accessibility Toolkit for elementary
Version: 1.0.0
Release: 1
License: LGPL
Group: Development/Libraries
Source: %{name}-%{version}.tar.xz
URL: http://www.gtk.org

BuildRequires: elementary
BuildRequires: atk
BuildRequires: eail
BuildRequires: elementary-devel
BuildRequires: atk-devel
BuildRequires: eail-devel

BuildRoot: %{buildroot}

%description
Handy library of accessibility functions. Development libs and headers
are in eail-devel.

%package devel
Summary: Header, docs and development files to to develop applications using eail.
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
Header, docs and development libraries for eail.

%prep
%setup -q

%build
make %{?jobs:-j%jobs}

%install

%clean
rm -rf %{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)

%files devel

%changelog
* Thu Jul 11 2013 packager@comarch.com
- Add eail as elementary module
* Wed Apr 17 2013 packager@comarch.com
- Fix spec file
* Mon Mar 25 2013 packager@comarch.com
- Initial (2.2.02.0)


