Name:       ug-setting-gallery-efl
Summary:    UG setting gallery ELF
Version:    1.0.32
Release:    1
Group:      Applications
License:    Flora Software License
Source0:    %{name}-%{version}.tar.gz
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

%build

%ifarch %{arm}
%define ARCH arm
%else
%define ARCH i586
%endif

export LDFLAGS+="-Wl,--rpath=%{_prefix}/lib -Wl,--as-needed"

LDFLAGS="$LDFLAGS" cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} -DARCH=%{ARCH}

make %{?jobs:-j%jobs}

%install
%make_install


%post
/sbin/ldconfig
export LD_LIBRARY_PATH=/usr/lib/ug:$LD_LIBRARY_PATH

/usr/bin/vconftool set -t double db/setting/gallery/interval_time 3.0 -g 6514
/usr/bin/vconftool set -t string db/setting/gallery/ss_effect "Slide" -g 6514
/usr/bin/vconftool set -t bool db/setting/gallery/repeat_state 0 -g 6514
/usr/bin/vconftool set -t bool db/setting/gallery/shuffle_state 0 -g 6514

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
/opt/ug/lib/*
/opt/ug/res/*
