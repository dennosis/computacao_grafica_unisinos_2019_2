#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <GLM/glm.hpp>
using namespace std;

class Material
{
public:
	Material() {}

	void addNOME(string x)
	{
		nome = x;
	}
	string getNOME(void)
	{
		return nome;
	}
	void addKA(glm::vec3 x)
	{
		ka = x;
	}
	glm::vec3 getKA(void) {
		return ka;
	}
	void addKD(glm::vec3 x)
	{
		kd = x;
	}
	glm::vec3 getKD(void) {
		return kd;
	}
	void addKS(glm::vec3 x)
	{
		ks = x;
	}
	glm::vec3 getKS(void) {
		return ks;
	}
	void addShiny(glm::vec3 x) {
		shiny = x;
	}
	glm::vec3 getShiny(void) {
		return shiny;
	}
	void addARQUIVO(string x)
	{
		arquivo = x;
	}
	string getArquivo(void)
	{
		return arquivo;
	}
	
private:
	string nome;
	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	glm::vec3 shiny;
	string arquivo;
};

class Face
{
public:
	Face() {}

    void addV(int x)
    {
		vertices.push_back(x);
    }
	int getV(int x)
	{
		return vertices.at(x);
	}
	void addN(int x)
	{
		normais.push_back(x);
	}
	void addT(int x)
	{
		texturas.push_back(x);
	}
private:
	vector<int> vertices;
	vector<int> normais;
	vector<int> texturas;
};

class Group
{
private:
	string nome;
	string material;
	int id;
	vector<Face*> faces;
public:
	Group() {}

	void setNome(string nom)
	{
		nome = nom;
	}
	string getNome(void)
	{
		return nome;
	}
	string getMaterial(void)
	{
		return material;
	}
	void setMaterial(string nom)
	{
		material = nom;
	}
	void setId(int nom)
	{
		id = nom;
	}
	int getId(void)
	{
		return id;
	}
	void addFaces(Face * aux)
	{
		faces.push_back(aux);
	}
	Face * getFace(int i) {
		return faces.at(i);
	}
};

class Mesh
{
private:
	vector<glm::vec3> vertices;
	vector<glm::vec3> normais;
	vector<glm::vec2> textures;
	vector<Group*> grupos;
	string nomearquivomaterial;
public:
	Mesh() {}
	vector<glm::vec3> getVector() {
		return vertices;
	}
	void setnomematerial(string x)
	{
		nomearquivomaterial = x;
	}
	string getnomematerial(void) {
		return nomearquivomaterial;
	}
	void addGrupo(Group * aux)
	{
		grupos.push_back(aux);
	}
	void addV(glm::vec3 x)
	{
		vertices.push_back(x);
	}
	void addN(glm::vec3 x)
	{
		normais.push_back(x);
	}
	void addT(glm::vec2 x)
	{
		textures.push_back(x);
	}
	glm::vec3 getIndV(int i) {
		return vertices.at(i);
	}
	glm::vec3 getIndN(int i) {
		return normais.at(i);
	}
	glm::vec2 getIndT(int i) {
		return textures.at(i);
	}
	Group * getGroup(int i) {
		return grupos.at(i);
	}
};

class ObjReader
{
private:
public:
	ObjReader() {}
	~ObjReader();

	Mesh *read(string filename) {
		Mesh *mesh = new Mesh;
		int cont = 0;
		int cont2 = 0;
		ifstream arq(filename);
		while (!arq.eof()) {
			string line;
			getline(arq, line);
			stringstream sline;
			sline << line;
			string temp;
			sline >> temp;
			if (temp == "v") {
				// ler vértice ...
				float x, y, z;
				sline >> x >> y >> z;
				mesh->addV(glm::vec3(x, y, z));
			}
			else if (temp == "vn") {
				//normais
				float x, y, z;
				sline >> x >> y >> z;
				mesh->addN(glm::vec3(x, y, z));
			}
			else if (temp == "vt") {
				//normais
				float x, y;
				sline >> x >> y;
				mesh->addT(glm::vec2(x, y));
			}
			else if (temp == "mtllib") {
				//normais
				string nomearq;
				sline >> nomearq;
				mesh->setnomematerial(nomearq);
			}
			else if (temp == "g") {
				string nomearq;
				sline >> nomearq;
				mesh->addGrupo(new Group());
				mesh->getGroup(cont)->setNome(nomearq);
			}
			else if (temp == "usemtl") {
				int nomearq;
				sline >> nomearq;
				mesh->getGroup(cont++)->setId(nomearq);
			}
			else if (temp == "f") {
				if (cont == 0) {
					mesh->addGrupo(new Group());
					mesh->getGroup(cont)->setNome("PADRAO");
					mesh->getGroup(cont++)->setId(0);
				}

				int x, y, z, w;
				sline >> x >> y >> z >> w;
				mesh->getGroup(cont - 1)->addFaces(new Face());
				mesh->getGroup(cont - 1)->getFace(cont2)->addV(x);
				mesh->getGroup(cont - 1)->getFace(cont2)->addV(y);
				mesh->getGroup(cont - 1)->getFace(cont2++)->addV(z);
			//	sline >> w;
			//	cout << w << " ";
				//cout << x << y << z;
				//cout << "\n";
			}
			else {// else-if
				  // Verificar outras possibilidades:
				  // g, vn, ...
			}
		}
		arq.close();
		return mesh;
	}

	void readermaterial(string filename, vector<Material*> &material) {
		int cont = 0;
		ifstream arq(filename);
		while (!arq.eof()) {
			string line;
			getline(arq, line);
			stringstream sline;
			sline << line;
			string temp;
			sline >> temp;
			if (temp == "newmtl") {
				// ler vértice ...
				string nomearq;
				sline >> nomearq;
				material.push_back(new Material());
				material.at(cont++)->addNOME(nomearq);
			}
			else if (temp == "map_Kd") {
				//normais
				string nomearq;
				sline >> nomearq;
				material.at(cont-1)->addARQUIVO(nomearq);
			}
			else if (temp == "Ka") {
				//normais
				float x, y, z;
				sline >> x >> y >> z;
				material.at(cont - 1)->addKA(glm::vec3(x, y, z));
			//	mesh->addT(glm::vec2(x, y));
			}
			else if (temp == "Kd") {
				//normais
				float x, y, z;
				sline >> x >> y >> z;
				material.at(cont - 1)->addKD(glm::vec3(x, y, z));
			}
			else if (temp == "Ks") {
				float x, y, z;
				sline >> x >> y >> z;
				material.at(cont - 1)->addKS(glm::vec3(x, y, z));
			}
			else if (temp == "Ns") {
				float x;
				sline >> x;
				material.at(cont - 1)->addShiny(glm::vec3(x, 0, 0));
			}
			else {// else-if
				  // Verificar outras possibilidades:
				  // g, vn, ...
			}
		}
		arq.close();
	}

};
