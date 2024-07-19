#include "MyForm.h"

using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

[STAThreadAttribute]
int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    CompletionistAutoUIPatcherGUI::MyForm form(args);
    Application::Run(% form);
    return 0;
}

//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------

namespace standard
{
    using namespace CompletionistAutoUIPatcherGUI;

    //---------------------------------------------------
    //---------------------------------------------------
    //---------------------------------------------------

    bool interfaceFolderExists() {
        // Get the path of the current executable
        String^ exePath = System::Windows::Forms::Application::StartupPath;

        // Combine with "interface" folder to check
        String^ interfaceFolderPath = Path::Combine(exePath, "interface");

        // Check if the "interface" folder exists
        if (Directory::Exists(interfaceFolderPath)) {
            return true;  // Installed within MO2 directory
        }
        return false;  // Not installed within MO2 directory
    }

    bool findFile(const std::string& directoryPath, const std::string& fileToFind) 
    {
        auto form = MyForm::GetInstance();

        try {
            // Convert fileToFind to lowercase (or uppercase)
            std::string fileToFindLower;
            std::transform(fileToFind.begin(), fileToFind.end(), std::back_inserter(fileToFindLower), [](unsigned char c) {
                return std::tolower(c);
                });

            // Iterate over the directory contents
            for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
                if (entry.is_regular_file()) {
                    // Get the filename as a native string and convert it to lowercase (or uppercase)
                    std::string filename = entry.path().filename().string();
                    std::transform(filename.begin(), filename.end(), filename.begin(), [](unsigned char c) {
                        return std::tolower(c);
                        });

                    // Check if the filenames match (case insensitive)
                    if (filename == fileToFindLower) {
                        return true;
                    }
                }
            }
        }
        catch (const std::exception& e) {
            std::string errorMessage = "Error iterating directory: ";
            errorMessage += e.what();
            form->LogMessage(errorMessage);
            return false;
        }

        return false; // File not found
    }

    //---------------------------------------------------
    //---------------------------------------------------
    //---------------------------------------------------

    bool findSWFFile(const std::string& directoryPath, const std::string& fileToFind, std::string& result) 
    {
        auto form = MyForm::GetInstance();

        try {
            // Convert fileToFind to lowercase (or uppercase)
            std::string fileToFindLower;
            std::transform(fileToFind.begin(), fileToFind.end(), std::back_inserter(fileToFindLower), [](unsigned char c) {
                return std::tolower(c);
                });

            // Iterate over the directory contents
            for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
                if (entry.is_regular_file()) {
                    // Get the filename as a native string and convert it to lowercase (or uppercase)
                    std::string filename = entry.path().filename().string();
                    std::transform(filename.begin(), filename.end(), filename.begin(), [](unsigned char c) {
                        return std::tolower(c);
                        });

                    // Check if the filenames match (case insensitive)
                    if (filename == fileToFindLower) {
                        result = entry.path().string();
                        return true;
                    }
                }
            }
        }
        catch (const std::exception& e) {
            std::string errorMessage = "Error iterating directory: ";
            errorMessage += e.what();
            form->LogMessage(errorMessage);
            return false;
        }

        return false; // File not found
    }

    //---------------------------------------------------
    //---------------------------------------------------
    //---------------------------------------------------

    bool createDirectory(const std::string& path) 
    {
        auto form = MyForm::GetInstance();

        try {
            std::filesystem::create_directories(path);
            return true;
        }
        catch (const std::exception& e) {
            std::string errorMessage = "Failed to create directory: " + path + ". Error: ";
            errorMessage += e.what();
            form->LogMessage(errorMessage);
            return false;
        }
    }

    //---------------------------------------------------
    //---------------------------------------------------
    //---------------------------------------------------

    void deleteDirectory(const std::string& directoryPath) 
    {
        auto form = MyForm::GetInstance();

        try {
            if (std::filesystem::exists(directoryPath)) {
                std::filesystem::remove_all(directoryPath);
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::string errorMessage = "Error deleting directory: " + directoryPath + ". Error: ";
            errorMessage += e.what();
            form->LogMessage(errorMessage);
        }
    }

    //---------------------------------------------------
    //---------------------------------------------------
    //---------------------------------------------------

    std::string readFileToString(const std::string& filename) 
    {
        auto form = MyForm::GetInstance();

        std::ifstream file(filename);
        if (!file.is_open()) 
        {
            form->LogMessage("Failed to open file: " + filename);
            return "";
        }

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        return content;
    }

    //---------------------------------------------------
    //---------------------------------------------------
    //---------------------------------------------------

    bool writeStringToFile(const std::string& filename, const std::string& content) 
    {
        auto form = MyForm::GetInstance();

        std::ofstream file(filename);
        if (!file.is_open()) 
        {
            form->LogMessage("Failed to open file for writing: " + filename);
            return false;
        }

        file << content;
        file.close();
        return true;
    }

    //---------------------------------------------------
    //---------------------------------------------------
    //---------------------------------------------------

    std::string insertBaseLinesAfter(std::string& content, const std::string& targetLine, const std::string& linesToInsert) 
    {
        auto form = MyForm::GetInstance();

        std::size_t pos = content.find(targetLine);
        if (pos != std::string::npos) {
            pos += targetLine.length(); // Move past the target line
            content.insert(pos, "\n" + linesToInsert); // Insert new lines after the target line
        }
        else {
            form->LogMessage("Unable to find line: " + targetLine);
            form->LogMessage("exiting...");
        }

        return content;
    }

    //---------------------------------------------------
    //---------------------------------------------------
    //---------------------------------------------------

    std::string insertBaseLinesAfterVariable(std::string& scriptContent, const std::string& variableName, const std::string& newLines) 
    {
        std::string targetLine = "var " + variableName + " = a_entryObject.text;";
        return insertBaseLinesAfter(scriptContent, targetLine, newLines);
    }

    //---------------------------------------------------
    //---------------------------------------------------
    //---------------------------------------------------

    std::string insertQuickLootLinesAfter(std::string& content, const std::string& targetLine, const std::string& linesToInsert)
    {
        std::string target = targetLine;

        auto form = MyForm::GetInstance();

        std::size_t pos = content.find(target);
        if (pos != std::string::npos) {
            pos += target.length(); // Move past the target line
            content.insert(pos, "\n" + linesToInsert); // Insert new lines after the target line
        }
        else {
            target = "LootMenu.ListItemRenderer.WHITE : LootMenu.ListItemRenderer.RED;";
            pos = content.find(target);
            if (pos != std::string::npos) {
                pos += target.length(); // Move past the target line
                content.insert(pos, "\n" + linesToInsert); // Insert new lines after the target line
            }
            else
            {
                form->LogMessage("Unable to find line: " + target);
                form->LogMessage("exiting...");
            }
        }

        return content;
    }

    //---------------------------------------------------
    //---------------------------------------------------
    //---------------------------------------------------

    std::string insertQuickLootLinesAfterVariable(std::string& scriptContent, const std::string& variableName, const std::string& newLines)
    {
        std::string targetLine = "var " + variableName + " = this.data.displayName == null ? \"\" : this.data.displayName;";
        return insertBaseLinesAfter(scriptContent, targetLine, newLines);
    }

    //---------------------------------------------------
    //---------------------------------------------------
    //---------------------------------------------------

    void QuickLootFormatter(const std::string& fileName, const std::string& className)
    {
        auto form = MyForm::GetInstance();

        if (!std::filesystem::exists(fileName)) {
            form->LogMessage("unable to locate swf file: " + fileName);
            form->LogMessage("exiting...");
            return;
        }

        std::string curWorkingDir = std::filesystem::current_path().string();
        std::string extractionDir = curWorkingDir + "\\extracted";
        std::string actionScriptFile = extractionDir + "\\scripts\\__Packages\\LootMenu\\" + className + ".as";
        std::string scriptName = "\\__Packages\\LootMenu\\" + className;
        std::string outPutFileDir{};
       
        if (form->toStdString(form->OutputFilesBox->Text) == "{Current Directory}/output") {
            outPutFileDir = curWorkingDir + "\\output";
        }
        else
        {
            outPutFileDir = form->toStdString(form->OutputFilesBox->Text);
        }

        outPutFileDir += form->CreateNewModFolder->Checked ? "\\mods\\Completionist UI Patcher Output\\interface" : "\\interface";

        // Ensure extraction directory exists
        if (!createDirectory(extractionDir)) {
            form->LogMessage("unable to create extraction directory: " + extractionDir);
            form->LogMessage("exiting...");
            return;
        }

        // Ensure output directory exists
        if (!createDirectory(outPutFileDir)) {
            form->LogMessage("unable to create output directory: " + outPutFileDir);
            form->LogMessage("exiting...");
            return;
        }

        // Step 1: Extract ActionScript using bundled FFDec
        std::string ffdecPath = "\"" + curWorkingDir + "\\ffdec\\ffdec-cli.exe";
        std::string extractCommand = "\"" + ffdecPath + "\" -export script \"" + extractionDir + "\" \"" + fileName + "\"";

        int extractResult = system(extractCommand.c_str());
        if (extractResult != 0) {
            deleteDirectory(extractionDir);
            form->LogMessage("Failed to extract ActionScript using FFDec. Error code: " + std::to_string(extractResult));
            form->LogMessage("exiting...");
            return;
        }

        // Step 2: Modify ActionScript file (CraftingListEntry.as)
        std::string scriptContent = readFileToString(actionScriptFile);
        if (scriptContent.empty()) {
            deleteDirectory(extractionDir);
            form->LogMessage("Failed to read ActionScript file: " + actionScriptFile);
            form->LogMessage("exiting...");
            return;
        }

        // Check if the script content already has completionist support
        if (scriptContent.find("CompTag") != std::string::npos) {
            deleteDirectory(extractionDir);
            form->LogMessage(MyForm::toStdString(MyForm::GetInstance()->CurrentFile) + " already has completionist support");
            form->LogMessage("exiting...");
            return;
        }

        // Define the regex pattern to find the line with 'var _locX_ = a_entryObject.text;'
        std::smatch match;
        std::regex pattern = std::regex(R"(var\s+([_a-zA-Z0-9]+)\s*=\s*this\.data\.displayName)");

        // Search for the pattern in the script content
        if (std::regex_search(scriptContent, match, pattern))
        {
            if (match.size() > 1)
            {
                std::string variableName = match.str(1); // Extracts _locX_ or similar variable name
                std::string newLines = "\n      var _comp_ = " + variableName + ".split(\"CompTag\");\n";
                newLines += "      " + variableName + " = _comp_[0].length > 0 ? _comp_[0] : " + variableName + ";\n";

                // Insert specific lines after a given line
                std::string additionalLines = R"(
      if(_comp_[1].length > 0)
      {
         this.textField.textColor = parseInt(_comp_[1]);
      }
            )";

                // Modify the script content
                std::string colorInsertString = "this.textField.textColor = rgb;";
                std::string modifiedContent = insertQuickLootLinesAfterVariable(scriptContent, variableName, newLines);
                modifiedContent = insertQuickLootLinesAfter(modifiedContent, colorInsertString, additionalLines);

                if (modifiedContent.empty()) {
                    deleteDirectory(extractionDir);
                    form->LogMessage("Failed to insert new lines into ActionScript.");
                    form->LogMessage("exiting...");
                    return;
                }

                // Write the modified content back to CraftingListEntry.as
                if (!writeStringToFile(actionScriptFile, modifiedContent)) {
                    deleteDirectory(extractionDir);
                    form->LogMessage("Failed to write modified content back to file: " + actionScriptFile);
                    form->LogMessage("exiting...");
                    return;
                }
            }
            else {
                deleteDirectory(extractionDir);
                form->LogMessage("Failed to extract variable name from pattern match.");
                form->LogMessage("exiting...");
                return;
            }
        }
        else {
            deleteDirectory(extractionDir);
            form->LogMessage("Pattern not found in ActionScript file.");
            form->LogMessage("exiting...");
            return;
        }

        //finished
        form->LogMessage("ActionScript modification successful...");

        std::string output = outPutFileDir + "\\" + MyForm::toStdString(MyForm::GetInstance()->CurrentFile);
        std::string replaceCommand = "\"" + ffdecPath + "\" -replace \"" + fileName + "\" \"" + output + "\" \"" + scriptName + "\" \"" + actionScriptFile + "\"";

        if (auto result = std::system(replaceCommand.c_str()) != 0) {
            deleteDirectory(extractionDir);
            form->LogMessage("Failed to execute FFDec replace command. Error code: " + std::to_string(result));
            form->LogMessage("exiting...");
            return;
        }

        form->LogMessage("SWF compiled successfully...");
        deleteDirectory(extractionDir);
    }

    //---------------------------------------------------
    //---------------------------------------------------
    //---------------------------------------------------

    void InventoryFormatter(const std::string& fileName, const std::string& className)
    {
        auto form = MyForm::GetInstance();

        if (!std::filesystem::exists(fileName)) {
            form->LogMessage("unable to locate swf file: " + fileName);
            form->LogMessage("exiting...");
            return;
        }

        std::string curWorkingDir = std::filesystem::current_path().string();
        std::string extractionDir = curWorkingDir + "\\extracted";
        std::string actionScriptFile = extractionDir + "\\scripts\\__Packages\\" + className + ".as";
        std::string scriptName = "\\__Packages\\" + className;
        std::string outPutFileDir{};

        if (form->toStdString(form->OutputFilesBox->Text) == "{Current Directory}/output") {
            outPutFileDir = curWorkingDir + "\\output";
        }
        else 
        {
            outPutFileDir = form->toStdString(form->OutputFilesBox->Text);
        }

        if (className == "InventoryListEntry") 
        {
            outPutFileDir += form->CreateNewModFolder->Checked ? "\\mods\\Completionist UI Patcher Output\\interface\\skyui" : "\\interface\\skyui";
        }
        else
        {
            outPutFileDir += form->CreateNewModFolder->Checked ? "\\mods\\Completionist UI Patcher Output\\interface" : "\\interface";
        }

        // Ensure extraction directory exists
        if (!createDirectory(extractionDir)) {
            form->LogMessage("unable to create extraction directory: " + extractionDir);
            form->LogMessage("exiting...");
            return;
        }

        // Ensure output directory exists
        if (!createDirectory(outPutFileDir)) {
            form->LogMessage("unable to create output directory: " + outPutFileDir);
            form->LogMessage("exiting...");
            return;
        }

        // Step 1: Extract ActionScript using bundled FFDec
        std::string ffdecPath = "\"" + curWorkingDir + "\\ffdec\\ffdec-cli.exe";
        std::string extractCommand = "\"" + ffdecPath + "\" -export script \"" + extractionDir + "\" \"" + fileName + "\"";

        int extractResult = system(extractCommand.c_str());
        if (extractResult != 0) {
            deleteDirectory(extractionDir);
            form->LogMessage("Failed to extract ActionScript using FFDec. Error code: " + std::to_string(extractResult));
            form->LogMessage("exiting...");
            return;
        }

        // Step 2: Modify ActionScript file (CraftingListEntry.as)
        std::string scriptContent = readFileToString(actionScriptFile);
        if (scriptContent.empty()) {
            deleteDirectory(extractionDir);
            form->LogMessage("Failed to read ActionScript file: " + actionScriptFile);
            form->LogMessage("exiting...");
            return;
        }

        // Check if the script content already has completionist support
        if (scriptContent.find("CompTag") != std::string::npos) {
            deleteDirectory(extractionDir);
            form->LogMessage(MyForm::toStdString(MyForm::GetInstance()->CurrentFile) + " already has completionist support");
            form->LogMessage("exiting...");
            return;
        }

        // Define the regex pattern to find the line with 'var _locX_ = a_entryObject.text;'
        std::smatch match;
        std::regex pattern = std::regex(R"(var\s+([_a-zA-Z0-9]+)\s*=\s*a_entryObject\.text;)");

        // Search for the pattern in the script content
        if (std::regex_search(scriptContent, match, pattern))
        {
            if (match.size() > 1)
            {
                std::string variableName = match.str(1); // Extracts _locX_ or similar variable name
                std::string newLines = "\n      var _comp_ = " + variableName + ".split(\"CompTag\");\n";
                newLines += "      " + variableName + " = _comp_[0].length > 0 ? _comp_[0] : " + variableName + ";\n";

                // Insert specific lines after a given line
                std::string additionalLines = R"(
      if(_comp_[1].length > 0)
      {
         a_entryField.textColor = parseInt(_comp_[1]);
      }
            )";

                // Modify the script content
                std::string colorInsertString = "this.formatColor(a_entryField,a_entryObject,a_state);";
                std::string modifiedContent = insertBaseLinesAfterVariable(scriptContent, variableName, newLines);
                modifiedContent = insertBaseLinesAfter(modifiedContent, colorInsertString, additionalLines);

                if (modifiedContent.empty()) {
                    deleteDirectory(extractionDir);
                    form->LogMessage("Failed to insert new lines into ActionScript.");
                    form->LogMessage("exiting...");
                    return;
                }

                // Write the modified content back to CraftingListEntry.as
                if (!writeStringToFile(actionScriptFile, modifiedContent)) {
                    deleteDirectory(extractionDir);
                    form->LogMessage("Failed to write modified content back to file: " + actionScriptFile);
                    form->LogMessage("exiting...");
                    return;
                }
            }
            else {
                deleteDirectory(extractionDir);
                form->LogMessage("Failed to extract variable name from pattern match.");
                form->LogMessage("exiting...");
                return;
            }
        }
        else {
            deleteDirectory(extractionDir);
            form->LogMessage("Pattern not found in ActionScript file.");
            form->LogMessage("exiting...");
            return;
        }

        //finished
        form->LogMessage("ActionScript modification successful...");
        
        std::string output = outPutFileDir + "\\" + MyForm::toStdString(MyForm::GetInstance()->CurrentFile);
        std::string replaceCommand = "\"" + ffdecPath + "\" -replace \"" + fileName + "\" \"" + output + "\" \"" + scriptName + "\" \"" + actionScriptFile + "\"";

        if (auto result = std::system(replaceCommand.c_str()) != 0) {
            deleteDirectory(extractionDir);
            form->LogMessage("Failed to execute FFDec replace command. Error code: " + std::to_string(result));
            form->LogMessage("exiting...");
            return;
        }

        form->LogMessage("SWF compiled successfully...");
        deleteDirectory(extractionDir);
    }
}