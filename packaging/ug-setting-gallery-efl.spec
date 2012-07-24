Name:       ug-setting-gallery-efl 
Summary:    UG setting gallery ELF    
Version:	1.0.17
Release:    1
Group:      TBD
License:    Flora Software License
Source0:    %{name}-%{version}.tar.gz
Source1001: packaging/ug-setting-gallery-efl.manifest 
BuildRequires: pkgconfig(bundle)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(ui-gadget)
BuildRequires: pkgconfig(vconf)
BuildRequires: cmake
BuildRequires: gettext-tools
Requires(post):   /sbin/ldconfig
Requires(post):   /usr/bin/vconftool
requires(postun): /sbin/ldconfig
%description
ug setting gallery efl. 

%prep
%setup -q -n %{name}-%{version}

%build
cp %{SOURCE1001} .
export LDFLAGS+="-Wl,--rpath=%{_prefix}/lib -Wl,--as-needed"

LDFLAGS="$LDFLAGS" cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}

make %{?jobs:-j%jobs}

%install
%make_install

%find_lang ug-setting-gallery-efl

%post 
/sbin/ldconfig 
/usr/bin/vconftool set -t double db/setting/gallery/interval_time 3.0 -g 6514
/usr/bin/vconftool set -t string db/setting/gallery/ss_effect "Slide" -g 6514
/usr/bin/vconftool set -t bool db/setting/gallery/repeat_state 0 -g 6514
/usr/bin/vconftool set -t bool db/setting/gallery/shuffle_state 0 -g 6514
/usr/bin/vconftool set -t bool db/setting/gallery/subtitle_state 0 -g 6514

%postun -p /sbin/ldconfig

%files -f ug-setting-gallery-efl.lang
%manifest ug-setting-gallery-efl.manifest
/opt/ug/lib/libug-setting-gallery-efl.so
/opt/ug/lib/libug-setting-gallery-efl.so.0.1.0

