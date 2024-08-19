#ifndef PARSERJSON_H
#define PARSERJSON_H

#include <fstream>
#include <vector>
#include <utility>
#include <wx/wx.h>

class ParserJSON {
public:
    ParserJSON() = default;
    ParserJSON(wxString fileName) {
        inputFile.open(fileName.ToStdString());
    }

    ~ParserJSON() {
        if(inputFile.is_open()) {
            inputFile.close();
        }

        if(outputFile.is_open()) {
            outputFile.close();
        }
    }

    bool openFile(wxString fileName) {
        if(inputFile.is_open()) {
            inputFile.close();
        }

        inputFile.open(fileName.ToStdString());

        return inputFile ? true : false;
    }

    bool parseJSONfile() {
        if(inputFile.is_open()) {
            std::string tempText;
            while (getline(inputFile, tempText)) {
                std::cout << tempText;
            }
            return true;
        }
        return false;
    }

    std::vector<std::pair<std::string, std::string>> getTaskList() const {
        return taskList;
    }

private:
    std::ifstream inputFile;
    std::ofstream outputFile;
    // Vettore di coppie "titolo":"descrizione" corrispondenti ad ogni task trovata nel file json
    std::vector<std::pair<std::string, std::string>> taskList;
};

#endif //PARSERJSON_H
