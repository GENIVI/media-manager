Name:       media-manager
Summary:    GENIVI Media Manager backend component
Version:    0.1
Release:    1
Group:      System/Libraries
License:    MPL-2.0
URL:        http://genivi.org
Source0:    %{name}-%{version}.tar.bz2

BuildRequires: pkgconfig(jansson)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(CommonAPI-DBus)
BuildRequires: pkgconfig(CommonAPI)
BuildRequires: dbus-genivi

BuildRequires: cmake
BuildRequires: git

%description
GENIVI Media Manager backend component. Acts as a server for the extension
component.

%package devel
Summary:    Development files for %{name}
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
GENIVI Media Manager development files

%package debug
Summary:    Debug files for %{name}
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description debug
GENIVI Media Manager debug files

%prep
%setup -q -n %{name}-%{version}
tar xvfz interfaces.tar.gz

%build
export PKG_CONFIG_PATH=/opt/genivi/lib/pkgconfig/
%cmake .

make %{?jobs:-j%jobs}

%install
%make_install

%files
/usr/bin/media-manager
