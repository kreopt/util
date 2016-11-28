Name: websock_rtsp_repo
Version: %{?_version}
Release: %{?_release}
#%{?dist}
Summary: Websocket RTSP Proxy repository
Group: Specforge
License: Specforge license
BuildArch: noarch
Source: %{name}-%{version}.%{release}.tar.gz

%define debug_package %{nil}

%description
no description 

%prep
%autosetup -c %{name}-%{version}.%{release}
#-q

%install

mkdir -p -m0755 $RPM_BUILD_ROOT/etc/
cp -rp install/etc/yum.repos.d/ $RPM_BUILD_ROOT/etc/

%post
ldconfig

%files
/etc/yum.repos.d/websock_rtsp.repo
