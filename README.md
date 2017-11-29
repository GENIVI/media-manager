README file for the media-manager project.
---
Copyright (C) Jaguar Land Rover 2014
Copyright (C) Pelagicore 2014

Media Manager
===
The purpose of a common Media Manager API is to provide to IVI
applications a way to control basic playback of contents of connected
CE devices. A single API enables a wider range of available devices
and media without implementing separate solutions for each type of
device.

Media Manager is composed of 3 main areas:
- Media Control Play Pause etc. including feedback for status play time etc.
- Media Indexer incuding Metadata discovery
- Media Browser Directory and Metadata Browsing - Artist album etc.

Structure of the code
===
The Media Manager project consists of the following sub-projects:
- media-manager (this is where you are now)
- media-manager-app
- media-manager-extension
- media-manager-interfaces

The media-manager project is the main project, and the software built from this
code will act as a CommonAPI server, providing interfaces for media playback,
browsing and indexer control.

The media-manager-app project provides a sample CrossWalk application, which is
used to control the media-manager. This application is written in JavaScript
and HTML, and can be seen as a UI.

The media-manager-extension project provides a CrossWalk extension which
communicates with media-manager over CommonAPI.

The media-manager-interfaces project contains interfaces shared between the
CommonAPI clients and server. This project contains Franca interfaces, and also
the CommonAPI code generated from the Franca interfaces. The generated code is
committed so that the media-manager can be built without having to use a
CommonAPI code generator on the building machine.

In order to try the "entire project", you need to build media-manager,
media-manager-app, and media-manager-extension. media-manager-interfaces is not
intended to be built.

Please see BUILDING in the respective project for more information on building.

More information here:
http://wiki.projects.genivi.org/index.php/Media_Manager

Getting in touch:
The main communication channel for the project is the mailing list. See the
below link for information on the mailing list. The archive is public, but
posting requires an application. The application process is purely to weed out
spam, anyone interested in the project is free to join the mailing list.

https://lists.genivi.org/mailman/listinfo/media-manager

Reporting bugs:
Please use the following Bugzilla for reporting bugs:
http://bugs.genivi.org/describecomponents.cgi?product=Media%20Manager

