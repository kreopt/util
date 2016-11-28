Name: websock_rtsp_proxy
Version: %{?_version}
Release: %{?_release}
#%{?dist}
Summary: Proxy server for streaming RTSP over websockets
Group: Specforge
License: Specforge license
BuildArch: x86_64 i686
Source: %{name}-%{version}.%{release}.tar.gz 
Requires: boost-system, boost-filesystem, boost-program-options, cpp >= 5.2, systemd

%define _unpackaged_files_terminate_build 0

%description
Proxy server for streaming RTSP over websockets

%prep
%autosetup -c %{name}-%{version}.%{release}
#-q

%build
cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/
#%cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/
make %{?_smp_mflags}

%install
make install DESTDIR=%{buildroot}

mkdir -p -m0755 $RPM_BUILD_ROOT/etc/
cp -rp install/etc/ws_rtsp.ini $RPM_BUILD_ROOT/etc/
cp -rp install/usr/ $RPM_BUILD_ROOT

%files
/usr/bin/websock_rtsp_proxy
/usr/lib/systemd/system/ws_rtsp.service
%config(noreplace) /etc/ws_rtsp.ini

%check
#ctest -V %{?_smp_mflags}
