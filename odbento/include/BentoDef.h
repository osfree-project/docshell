/**************************************************************************
 *
 *  Copyright 2008, Roger Brown
 *
 *  This file is part of Roger Brown's Toolkit.
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

/*
 * $Id$
 */

#ifndef _BENTODEF_
#define _BENTODEF_

#define kODContainerBento 			"OpenDoc:Apple:Bento"

#define kODBentoContainerID 		"ODBentoContainer"
#define kODFileContainerID 			"ODFileContainer"
#define kODMemoryContainerID 		"ODMemContainer"
#define kODEmbeddedContainerID 		"ODEmbeddedContainer"
#define kODUpdateContainerID		"ODUpdateContainer"

#define kODBentoEmbeddedContainer	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:ContainerType:Embedded"
#define kODBentoFileContainer		"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:ContainerType:File"
#define kODBentoMemoryContainer		"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:ContainerType:Memory"
#define kODBentoUpdateContainer		"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:ContainerType:Update"

#define kODPropVersionList      	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Property:DraftList"
#define kODPropStorageUnitName    	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Metadata:StorageUnitName"
#define kODPropCurrentSURef      	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Metadata:CurrentSURef"
#define kODPropDocumentName      	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Property:DocumentName"

#define kNMAPid1 128

#define kcfrgID				0

#define kODBentoPrefix	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:"
#define kODValue		"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:Bytes"
#define kODFrame		"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:Frame"
#define kODPart 		"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:Part"
#define kODLink 		"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:Link"
#define kODLinkSource	"+//ISO 9070/ANSI::113722::US::CI LABS::OpenDoc:Bento Container Suite:Type:LinkSource"

#define	kODErrBentoErr 						-29809
#define kODErrInvalidVersion				-29808
#define kODErrCannotCreateDraftVersion		-29807
#define kODErrVersionListUnavailable		-29806
#define kODErrSemaphoreReleased				-29805
#define kODErrInvalidIDList					-29803
#define kODErrBentoNoSession				-29802
#define kODErrBentoInvalidProperty			-29801
#define kODErrBentoInvalidType				-29800
#define kODErrBentoCannotNewObject			-29799
#define kODErrBentoCannotNewValue			-29798
#define kODErrBentoCannotCreateContainer	-29797
#define kODErrBentoCannotOpenContainer		-29796
#define kODErrBentoInvalidObject			-29795
#define kODErrDraftExistsWithDifferentID	-29794
#define kODErrNoVersionList					-29793
#define kODErrNotPromise					-29792
#define kODErrInvalidZone					-29791
#define kODErrCannotGetDraftVersion			-29790
#define kODErrNoDocumentProperties			-29789

#endif

