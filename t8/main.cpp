//Crístian Marcos Weber
//201610448

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "window.h"
#include <FL/fl_ask.H>

using namespace std;

class Campo {
private:
  std::string label;
  std::string id;
public:
  Campo(std::string label, std::string id) {
    this->label = label;
    this->id = id;
  }

  std::string getLabel() {
    return label;
  }

  std::string getId() {
    return id;
  }

  void setLabel(std::string label) {
    this->label = label;
  }

  void setId(std::string id) {
    this->id = id;
  }

};

class Generator : public UserInterface {
private:
  std::vector<Campo> campos;
  std::string titulo;
public:

  Generator(){
    okBtn->callback((Fl_Callback*) cbOkBtn, (void*)(this));
    insertBtn->callback((Fl_Callback*) cbInsertBtn, (void*)(this));
    updateBtn->callback((Fl_Callback*) cbUpdateBtn, (void*)(this));
    deleteBtn->callback((Fl_Callback*) cbDeleteBtn, (void*)(this));
    clearBtn->callback((Fl_Callback*) cbClearBtn, (void*)(this));
    dataBrowser->callback((Fl_Callback*) cbBrowser, (void*)(this));
    importBtn->callback((Fl_Callback*) cbImportBtn, (void*)(this));
    exportBtn->callback((Fl_Callback*) cbExportBtn, (void*)(this));
    generateBtn->callback((Fl_Callback*) cbGenerateBtn, (void*)(this));
  }

  static void cbBrowser(Fl_Widget *Btn, void* userdata) {
    Generator* gui = static_cast<Generator*>(userdata);
    int index = gui->dataBrowser->value();
    gui->labelInput->value(gui->campos[index-1].getLabel().c_str());
    gui->idInput->value((gui->campos[index-1].getId()).c_str());
  }

  static void cbOkBtn(Fl_Widget* Btn, void* userdata){
    Generator* gui = static_cast<Generator*>(userdata);
    if(strcmp(gui->titleInput->value(),"") != 0){
      gui->titleBox->label(gui->titleInput->value());
      gui->titulo = string(gui->titleInput->value());
      gui->titleInput->value("");
    }
  }

  static void cbInsertBtn(Fl_Widget* Btn, void* userdata){
    Generator* gui = static_cast<Generator*>(userdata);
    std::string label = string(gui->labelInput->value());
    std::string id = string(gui->idInput->value());
    if(label != "" && id != ""){
      gui->campos.push_back(Campo(label,id));
      gui->dataBrowser->add((id + " | " + label).c_str());
      gui->idInput->value("");
      gui->labelInput->value("");

    }
  }

  static void cbUpdateBtn(Fl_Widget* Btn, void* userdata){
    Generator* gui = static_cast<Generator*>(userdata);
    int index = gui->dataBrowser->value();
    if(index > 0){
      std::string label = string(gui->labelInput->value());
      std::string id = string(gui->idInput->value());
      if(label != "" && id != ""){
        gui->campos[index-1].setLabel(label);
        gui->campos[index-1].setId(id);
        gui->dataBrowser->text(index,(id + " | " + label).c_str());
        gui->labelInput->value("");
        gui->idInput->value("");
      }
    }
  }

  static void cbDeleteBtn(Fl_Widget* Btn, void* userdata){
    Generator* gui = static_cast<Generator*>(userdata);
    int index = gui->dataBrowser->value();
    if(index > 0){
      gui->campos.erase(gui->campos.begin() + index-1);
      gui->dataBrowser->remove(index);
      gui->idInput->value("");
      gui->labelInput->value("");
    }
  }

  static void cbClearBtn(Fl_Widget* Btn, void* userdata){
    Generator* gui = static_cast<Generator*>(userdata);
    gui->titleInput->value("");
    gui->idInput->value("");
    gui->labelInput->value("");
  }

  static void cbImportBtn(Fl_Widget* Btn, void* userdata){
    Generator *gui = static_cast<Generator*>(userdata);
    std::string file = string(gui->fileInput->value());
    std::ifstream entrada("projetos/" + file + ".csv");
    if(entrada.is_open()){
       for(int i = gui->campos.size() -1; i >= 0; i--){
	  gui->campos.pop_back();
          gui->dataBrowser->remove(i+1);
       }
       gui->titulo = "";
       gui->titleBox->label("");

	std::string line;
        std::string splited[2];
	getline(entrada, line);
	gui->titulo = line;
	while(!entrada.eof()){
	  getline(entrada,line);
	  if(!line.empty()){
	    std::stringstream linestream(line);
	    getline(linestream,splited[0], ',');
	    getline(linestream,splited[1], ',');
	    gui->campos.push_back(Campo(splited[1], splited[0]));
	  }
	}
	gui->titleBox->label((gui->titulo).c_str());
	for(int i=0 ; i < gui->campos.size() ; i++){
	  gui->dataBrowser->add((gui->campos[i].getId() + " | " + gui->campos[i].getLabel()).c_str());
	}
    }
    else{
	fl_alert("Projeto não encontrado");
    }
  }

  static void cbExportBtn(Fl_Widget* Btn, void* userdata){
    Generator *gui = static_cast<Generator*>(userdata);
    std:string file = string(gui->fileInput->value());
    if(file != ""){
      if(gui->titulo == "") fl_alert("Insira um titulo para seu projeto");
      else{
        std::ofstream saida("projetos/" + file + ".csv");
        saida << gui->titulo << std::endl;
        for(int i = 0 ; i < gui->campos.size(); i++){
          saida << gui->campos[i].getId() << "," << gui->campos[i].getLabel() << endl;
        }
      }
    }else{
      fl_alert("Insira o nome do arquivo a ser salvo");
    }
  }

  void insere_3(ofstream &output){
    for(int i = 0; i < campos.size(); i++){
      output << "<div class=\"form-group\">\n"
      << "<label for=\"" << campos[i].getId() << "\" class=\"col-sm-2 control-label\">" << campos[i].getLabel() << "</label>\n"
      << "<div class=\"col-sm-6\">\n"
      << "<input type=\"text\" class=\"form-control validate[required]\" id=\"" << campos[i].getId() << "\" placeholder=\"" << campos[i].getLabel() << "\">\n"
      << "</div>\n"
      << "</div>\n";
    }
  }

  void insere_4(ofstream &output){
    output << "dataSet[seq]=[index,icons";
    for(int i = 0; i < campos.size(); i++){
      output << ", " << campos[i].getId();
    }
    output << "];\n";
  }

  void insere_5(ofstream &output){
    output << "oTable=$('#table').dataTable({\n"
    <<"\"data\": dataSet,\n"
    <<"\"columns\": [\n"
    <<"{ \"title\": \"Seq\", \"class\": \"center\" },\n"
    <<"{ \"title\": \"\", \"class\": \"center\" },\n";
    for(int i = 0; i < campos.size(); i++){
	output << "{\"title\": \"" << campos[i].getLabel() << "\"},\n";
    }
    output << "]\n" << "});\n";
  }

  void insere_6(ofstream &output){
    output << "$('#table tbody').on( 'click', '.glyphicon-edit', function () {\n"
    << "$('#list').hide();\n"
    << "var rows = $(this).parents('tr').children();\n"
    << "$('#seq').text(rows[0].innerHTML);\n";
    for(int i = 0; i < campos.size(); i++){
      output << "$('#" << campos[i].getId() << "').val(rows[" << i+2 << "].innerHTML);\n";
    }
    output << "$('#formID').show()\n;" << "} );";
  }

  void insere_7(ofstream &output){
    output << "function save() {\n" << "var seq=$('#seq').text();\n"
    << "if ( isNaN(seq) ) { seq=0; }\n" << "var key=formId+\"_\"+seq;\n" << "var object={\n";
    for(int i = 0; i < campos.size(); i++){
      output <<  "\"" << campos[i].getId() << "\": $(\"#" <<campos[i].getId() << "\").val(),\n";
    }
    output <<"}\n"<< "localStorage.setItem(key, JSON.stringify(object));\n" << "}\n";
  }

  static void cbGenerateBtn(Fl_Widget* Btn, void* userdata){
    Generator* gui = static_cast<Generator*>(userdata);
    std::ifstream input("template.html");
    std::ofstream output("index.html");
    if(input.is_open()){
      std::string line;
      while(!input.eof()){
	getline(input,line);
	if(line == ">>>1<<<") output << "<title>" << gui->titulo << "</title>" << std::endl;
	else if(line == ">>>2<<<") output << "<h1>" << gui->titulo << "</h1>" << std::endl;
	else if(line == ">>>3<<<") gui->insere_3(output);
	else if(line == ">>>4<<<") gui->insere_4(output);
	else if(line == ">>>5<<<") gui->insere_5(output);
	else if(line == ">>>6<<<") gui->insere_6(output);
	else if(line == ">>>7<<<") gui->insere_7(output);
	else output << line << std::endl;
      }
    }
  }

  void show(){
    w->show();
  }

};

int main() {
  Generator gui;
  gui.show();
  return Fl::run();
}
