#include "labels.h"
#include "utils.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>
#include <QColormap>
#include <QDebug>

LabelInfo::LabelInfo() {
	this->name = "unlabeled";
	this->categorie = "void";
	this->id = 0;
	this->id_categorie = 0;
	this->color = QColor(0, 0, 0);
	item = NULL;
}

LabelInfo::LabelInfo(QString name, QString categorie, int id, int id_categorie, QColor color) {
	this->name = name;
	this->categorie = categorie;
	this->id = id;
	this->id_categorie = id_categorie;
	this->color = color;
	item = NULL;
}

void LabelInfo::read(const QJsonObject &json) {
	id = json["id"].toInt();
	name = json["name"].toString();
	categorie = json["categorie"].toString();
	id_categorie = json["id_categorie"].toInt();
	QJsonArray jarray = json["color"].toArray();
	int r = jarray[0].toInt();
	int g = jarray[1].toInt();
	int b = jarray[2].toInt();
	color = QColor(r, g, b);
}

void LabelInfo::write(QJsonObject &json) const {
	json["id"] = id;
	json["name"] = name;
	json["categorie"] = categorie;
	json["id_categorie"] = id_categorie;
	QJsonArray jarray;
	jarray.append(color.red());
	jarray.append(color.green());
	jarray.append(color.blue());
	json["color"] = jarray;
}

void Name2Labels::read(const QJsonObject &json) {
	QJsonObject array = json["labels"].toObject();
	for (QJsonObject::iterator it = array.begin(); it != array.end(); it++) {
		QJsonObject object = it->toObject();
		LabelInfo label;
		label.read(object);
		(*this)[label.name] = label;
	}
}

void Name2Labels::write(QJsonObject &json) const {
	QMapIterator<QString, LabelInfo> it(*this);
	QJsonObject object;
	while (it.hasNext()) {
		it.next();
		const LabelInfo & label = it.value();
		QJsonObject child_object;
		label.write(child_object);
		object[it.key()] = child_object;
	}
	json["labels"] = object;
}


Id2Labels getId2Label(const Name2Labels& labels) {
	Id2Labels id_labels;
	QMapIterator<QString, LabelInfo> i(labels);
	while (i.hasNext()) {
		i.next();
		id_labels[i.value().id] = &i.value();
	}
	return id_labels;
}

Name2Labels defaultLabels() {
	Name2Labels labels;
	// inspired by https://raw.githubusercontent.com/mcordts/cityscapesScripts/master/cityscapesscripts/helpers/labels.py
	labels["other"               ] = LabelInfo("other"               , "void"         ,  00, 0, QColor(  0,  0,  0));
	labels["building"            ] = LabelInfo("building"            , "construction" ,  01, 2, QColor( 70, 70, 70));
	labels["collection container"] = LabelInfo("collection container", "construction" ,  02, 2, QColor(  0, 60,100));
	labels["debris"              ] = LabelInfo("debris"              , "object"       ,  03, 3, QColor(153,153,153));
	labels["landing site"        ] = LabelInfo("landing site"        , "construction" ,  04, 2, QColor(255,  0,  0));
	labels["path"                ] = LabelInfo("path"                , "flat"         ,  05, 1, QColor(250,170, 30));
	labels["person"              ] = LabelInfo("person"              , "human"        ,  06, 6, QColor(220, 20, 60));
	labels["rock"                ] = LabelInfo("rock"                , "object"       ,  07, 3, QColor(153,153,153));
	labels["sign"                ] = LabelInfo("sign"                , "object"       ,   8, 3, QColor(220,220,  0));
	labels["sky"                 ] = LabelInfo("sky"                 , "sky"          ,   9, 5, QColor( 70,130,180));
	labels["slash"               ] = LabelInfo("slash"               , "object"       ,  10, 0, QColor(111, 74,  0));
	labels["terrain"             ] = LabelInfo("terrain"             , "nature"       ,  11, 4, QColor(152,251,152));
	labels["trailer"             ] = LabelInfo("trailer"             , "vehicle"      ,  12, 7, QColor(  0,  0,110));
	labels["tree"                ] = LabelInfo("tree"                , "object"       ,  13, 3, QColor(119, 11, 32));
	labels["vegetation"          ] = LabelInfo("vegetation"          , "nature"       ,  14, 4, QColor(107,142, 35));
	labels["vehicle"             ] = LabelInfo("vehicle"             , "vehicle"      ,  15, 7, QColor(  0,  0, 70));
	labels["water"               ] = LabelInfo("water"               , "nature"       ,  16, 4, QColor(  0,  0,230));
	return labels;
}
