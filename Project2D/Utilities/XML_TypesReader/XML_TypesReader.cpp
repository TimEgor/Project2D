#include "XML_TypesReader.h"

//void xmlReadVector_XMFLOAT2(const pugi::xml_node & xmlNode, DirectX::XMFLOAT2 & vector)
//{
//	pugi::xml_node coord = xmlNode.child("X");
//	if (coord)
//		vector.x = coord.text().as_float();
//
//	coord = xmlNode.child("Y");
//	if (coord)
//		vector.y = coord.text().as_float();
//}
//
//void xmlReadVector_XMFLOAT3(const pugi::xml_node & xmlNode, DirectX::XMFLOAT3 & vector)
//{
//	pugi::xml_node coord = xmlNode.child("X");
//	if (coord)
//		vector.x = coord.text().as_float();
//
//	coord = xmlNode.child("Y");
//	if (coord)
//		vector.y = coord.text().as_float();
//
//	coord = xmlNode.child("Z");
//	if (coord)
//		vector.z = coord.text().as_float();
//}
//
//void xmlReadVector_XMFLOAT4(const pugi::xml_node & xmlNode, DirectX::XMFLOAT4 & vector)
//{
//	pugi::xml_node coord = xmlNode.child("X");
//	if (coord)
//		vector.x = coord.text().as_float();
//
//	coord = xmlNode.child("Y");
//	if (coord)
//		vector.y = coord.text().as_float();
//
//	coord = xmlNode.child("Z");
//	if (coord)
//		vector.z = coord.text().as_float();
//
//	coord = xmlNode.child("W");
//	if (coord)
//		vector.w = coord.text().as_float();
//}
//
//void xmlReadVector_XMFLOAT3X3(const pugi::xml_node &xmlNode, DirectX::XMFLOAT3X3 &matrix)
//{
//	float *address = (float*)&matrix;
//
//	pugi::xml_node coord;
//	for (size_t i = 0; i < 3; ++i)
//		for (size_t j = 0; j < 3; ++j)
//		{
//			char coordName[] = "11";
//			coordName[0] += i;
//			coordName[1] += j;
//
//			coord = xmlNode.child(coordName);
//			if (coord)
//				address[i * 3 + j] = coord.text().as_float();
//		}
//}
//
//void xmlReadVector_XMFLOAT4X4(const pugi::xml_node &xmlNode, DirectX::XMFLOAT4X4 &matrix)
//{
//	float *address = (float *)& matrix;
//
//	pugi::xml_node coord;
//	for (size_t i = 0; i < 4; ++i)
//		for (size_t j = 0; j < 4; ++j)
//		{
//			char coordName[] = "11";
//			coordName[0] += i;
//			coordName[1] += j;
//
//			coord = xmlNode.child(coordName);
//			if (coord)
//				address[i * 4 + j] = coord.text().as_float();
//		}
//}

//void xmlReadColorRGB(const pugi::xml_node & xmlNode, ColorRGB & color)
//{
//	pugi::xml_node coord = xmlNode.child("R");
//	if (coord)
//		color.r = coord.text().as_float();
//
//	coord = xmlNode.child("G");
//	if (coord)
//		color.g = coord.text().as_float();
//
//	coord = xmlNode.child("B");
//	if (coord)
//		color.b = coord.text().as_float();
//}
//
//void xmlReadRGBA(const pugi::xml_node & xmlNode, ColorRGBA & color)
//{
//	pugi::xml_node coord = xmlNode.child("R");
//	if (coord)
//		color.r = coord.text().as_float();
//
//	coord = xmlNode.child("G");
//	if (coord)
//		color.g = coord.text().as_float();
//
//	coord = xmlNode.child("B");
//	if (coord)
//		color.b = coord.text().as_float();
//
//	coord = xmlNode.child("A");
//	if (coord)
//		color.a = coord.text().as_float();
//}
