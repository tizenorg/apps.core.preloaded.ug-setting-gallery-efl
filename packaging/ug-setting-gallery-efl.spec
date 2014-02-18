%bcond_with wayland

Name:       ug-setting-gallery-efl
Summary:    UG setting gallery ELF
Version:    1.0.43
Release:    1
Group:      Applications
License:    Flora Software License
Source0:    %{name}-%{version}.tar.gz
Source1001: 	ug-setting-gallery-efl.manifest
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(edje)
BuildRequires: pkgconfig(eina)
BuildRequires: pkgconfig(evas)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(ui-gadget-1)
BuildRequires: cmake
BuildRequires: gettext
Requires(post):   /sbin/ldconfig
Requires(post):   /usr/bin/vconftool
requires(postun): /sbin/ldconfig
%description
ug setting gallery efl.

%prep
%setup -q -n %{name}-%{version}
cp %{SOURCE1001} .

%build

%ifarch %{arm}
%define ARCH arm
%else
%define ARCH i586
%endif

export LDFLAGS+="-Wl,--rpath=%{_prefix}/lib -Wl,--as-needed"

LDFLAGS="$LDFLAGS" %cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} -DARCH=%{ARCH} \
%if %{with wayland}
         -DWAYLAND_SUPPORT=On
%else
         -DWAYLAND_SUPPORT=Off
%endif


make %{?jobs:-j%jobs}

%install
%make_install
mkdir -p %{buildroot}%{_datarootdir}/license

%post
/sbin/ldconfig
export LD_LIBRARY_PATH=%{_prefix}/lib/ug:$LD_LIBRARY_PATH

/usr/bin/vconftool set -t double db/gallery/setting/interval_time 3.0 -g 6514
/usr/bin/vconftool set -t bool db/gallery/setting/repeat_state 0 -g 6514
/usr/bin/vconftool set -t bool db/gallery/setting/shuffle_state 0 -g 6514
/usr/bin/vconftool set -t string db/gallery/setting/ss_effect "Slide" -g 6514

%postun -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_prefix}/ug/lib/*
%{_prefix}/ug/res/*
%license %{_datarootdir}/license/%{name}

