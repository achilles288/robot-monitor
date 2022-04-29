all:
	@ mkdir -p build/deb
	@ mkdir -p build/bionic
	@ mkdir -p build/focal
	@ cp -r librmonitor build/deb
	@ cp -r ../../station/* build/deb/librmonitor/src
	@ cp -r build/deb/librmonitor build/focal
	@ cp -r build/deb/librmonitor build/bionic
	@ cp -r changelog build/deb/librmonitor/debian/changelog
	@ cp -r changelog.bionic build/bionic/librmonitor/debian/changelog
	@ cp -r changelog.focal build/focal/librmonitor/debian/changelog
