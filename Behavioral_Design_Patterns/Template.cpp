#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

class ReportData {
public:
    vector<string> getHeaders() {
      vector<string> headers = {"ID", "Name", "Value"};
      return headers;
    }
    
    vector<map<string, string>> getRows() {
      vector<map<string, string>> rows;
      
      map<string, string> row1;
      row1["ID"] = "1";
      row1["Name"] = "Item A";
      row1["Value"] = "100.0";
      rows.push_back(row1);
      
      map<string, string> row2;
      row2["ID"] = "2";
      row2["Name"] = "Item B";
      row2["Value"] = "150.5";
      rows.push_back(row2);
      
      map<string, string> row3;
      row3["ID"] = "3";
      row3["Name"] = "Item C";
      row3["Value"] = "75.25";
      rows.push_back(row3);
      
      return rows;
    }
};

class AbstractReportExporter {
public:
    virtual ~AbstractReportExporter() = default;

    // Template method - not virtual, subclasses cannot override
    void exportReport(ReportData* data, string filePath) {
        prepareData(data);
        openFile(filePath);
        writeHeader(data);
        writeDataRows(data);
        writeFooter(data);
        closeFile(filePath);
        cout << "Export complete: " << filePath << endl;
    }

protected:
	// Concrete method - shared across all subclasses
    virtual void prepareData(ReportData* data) {
        cout << "Preparing report data..." << endl;
    }

	// Concrete method - shared across all subclasses
    virtual void openFile(string filePath) {
        cout << "Opening file: " << filePath << endl;
    }

	// Abstract method - each subclass MUST implement
    virtual void writeHeader(ReportData* data) = 0;

	// Abstract method - each subclass MUST implement
    virtual void writeDataRows(ReportData* data) = 0;

    // Hook method - optional override
    virtual void writeFooter(ReportData* data) {
        // Default: no footer
    }

	// Concrete method - shared across all subclasses
    virtual void closeFile(string filePath) {
        cout << "Closing file: " << filePath << endl;
    }
};

class CsvReportExporter : public AbstractReportExporter {
protected:
    void writeHeader(ReportData* data) override {
        auto headers = data->getHeaders();
        cout << "CSV: ";
        for (size_t i = 0; i < headers.size(); i++) {
            cout << headers[i];
            if (i < headers.size() - 1) cout << ",";
        }
        cout << endl;
    }

    void writeDataRows(ReportData* data) override {
        auto headers = data->getHeaders();
        for (auto& row : data->getRows()) {
            cout << "CSV: ";
            for (size_t i = 0; i < headers.size(); i++) {
                cout << row[headers[i]];
                if (i < headers.size() - 1) cout << ",";
            }
            cout << endl;
        }
    }
};

class PdfReportExporter : public AbstractReportExporter {
protected:
    void writeHeader(ReportData* data) override {
        auto headers = data->getHeaders();
        cout << "PDF: | ";
        for (auto& h : headers) cout << h << " | ";
        cout << endl;
        cout << "PDF: " << string(40, '-') << endl;
    }

    void writeDataRows(ReportData* data) override {
        auto headers = data->getHeaders();
        for (auto& row : data->getRows()) {
            cout << "PDF: | ";
            for (auto& h : headers) cout << row[h] << " | ";
            cout << endl;
        }
    }

    void writeFooter(ReportData* data) override {
        cout << "PDF: --- Page 1 of 1 ---" << endl;
    }
};

int main() {
    ReportData data;

    CsvReportExporter csvExporter;
    csvExporter.exportReport(&data, "sales_report.csv");

    cout << endl;

    PdfReportExporter pdfExporter;
    pdfExporter.exportReport(&data, "sales_report.pdf");

    return 0;
}
