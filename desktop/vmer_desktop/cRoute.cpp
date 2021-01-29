#include "cRoute.h"

cPoint::cPoint(const int _id, const int _element_id, const QString _name, const QString _config, const QString _desc)
    : id(_id), element_id(_element_id), name(_name), config(_config), desc(_desc)
{
    item.setText(0, _name);
    item.setWhatsThis(0, "Point");
}
cPoint::~cPoint() {

}

cElement::cElement(const int _id, const QString _name, const QString _image, const QString _desc)
    : id(_id), name(_name), image(_image), desc(_desc)
{
    item.setText(0, _name);
    item.setWhatsThis(0, "Element");
}
