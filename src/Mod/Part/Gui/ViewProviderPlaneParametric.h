/***************************************************************************
 *   Copyright (c) 2004 Jürgen Riegel <juergen.riegel@web.de>              *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#ifndef PARTGUI_VIEWPROVIDERPLANEPARAMETRIC_H
#define PARTGUI_VIEWPROVIDERPLANEPARAMETRIC_H

#include "ViewProviderPrimitive.h"


class TopoDS_Shape;
class TopoDS_Face;
class SoSeparator;
class SbVec3f;
class SoTransform;

namespace PartGui {


class PartGuiExport ViewProviderPlaneParametric : public ViewProviderPrimitive
{
    PROPERTY_HEADER_WITH_OVERRIDE(PartGui::ViewProviderPlaneParametric);

public:
    /// constructor
    ViewProviderPlaneParametric();
    /// destructor
    ~ViewProviderPlaneParametric() override;

    bool canReplaceObject(App::DocumentObject*, App::DocumentObject*) override {
        return true;
    }
    std::vector<std::string> getDisplayModes() const override;

protected:

};

class PartGuiExport ViewProviderFace : public ViewProviderPlaneParametric
{
    PROPERTY_HEADER_WITH_OVERRIDE(PartGui::ViewProviderFace);

public:
    ViewProviderFace();
    ~ViewProviderFace() override;

    std::vector<App::DocumentObject*> claimChildren() const override;
    bool canDragObjects() const override;
    bool canDragObject(App::DocumentObject*) const override;
    void dragObject(App::DocumentObject*) override;
    bool canDropObjects() const override;
    bool canDropObject(App::DocumentObject*) const override;
    void dropObject(App::DocumentObject*) override;
};

} // namespace PartGui


#endif // PARTGUI_VIEWPROVIDERPLANEPARAMETRIC_H

