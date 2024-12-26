#pragma once
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // For system() function
#include <windows.h> // For Windows API
#include <commdlg.h> // For file open dialog
#include <shlobj.h> // For directory selection
#include <filesystem> // For file operations
#include <regex> // For regex support
#include <sstream>
#include <vcclr.h>
#include <msclr/marshal_cppstd.h>

namespace standard 
{
	static bool interfaceFolderExists();

	static void InventoryFormatter(const std::string & fileName, const std::string& className);
	static void QuickLootFormatter(const std::string& fileName, const std::string& className);

	static bool findFile(const std::string& directoryPath, const std::string& fileToFind);
	static bool findSWFFile(const std::string& directoryPath, const std::string& fileToFind, std::string& result);
	static bool createDirectory(const std::string& path);
	static void deleteDirectory(const std::string& directoryPath);
	static bool writeStringToFile(const std::string& filename, const std::string& content);
	static std::string readFileToString(const std::string& filename);

	static std::string insertBaseLinesAfter(std::string& content, const std::string& targetLine, const std::string& linesToInsert);
	static std::string insertBaseLinesAfterVariable(std::string& scriptContent, const std::string& variableName, const std::string& newLines);

	static std::string insertQuickLootLinesAfter(std::string& content, const std::string& targetLine, const std::string& linesToInsert);
	static std::string insertQuickLootLinesAfterVariable(std::string& scriptContent, const std::string& variableName, const std::string& newLines);
};

namespace CompletionistAutoUIPatcherGUI 
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Text;
	using namespace System::IO;
	using namespace ComponentFactory::Krypton::Toolkit;

	public ref class MyForm : public KryptonForm
	{
	public:
		static MyForm^ instance;

	private: ComponentFactory::Krypton::Toolkit::KryptonPalette^ kryptonPalette1;
	private: System::Windows::Forms::Label^ label1;

	private: ComponentFactory::Krypton::Toolkit::KryptonTextBox^ InventoryListsFilePathBox;

	private: ComponentFactory::Krypton::Toolkit::KryptonButton^ RunButton;
	private: ComponentFactory::Krypton::Toolkit::KryptonTextBox^ CraftingMenuFilePathBox;
	private: ComponentFactory::Krypton::Toolkit::KryptonTextBox^ QuickLootFilePathBox;
	private: ComponentFactory::Krypton::Toolkit::KryptonTextBox^ ConstructibleObjectMenuFilePathBox;
	private: ComponentFactory::Krypton::Toolkit::KryptonButton^ CopyLog;

	private: ComponentFactory::Krypton::Toolkit::KryptonRichTextBox^ log;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	public: ComponentFactory::Krypton::Toolkit::KryptonTextBox^ OutputFilesBox;

	private: bool suppressEnterEvent = false;
	private: System::Windows::Forms::PictureBox^ InventoryListsFilePathBox_Image;
	private: System::Windows::Forms::PictureBox^ CraftingMenuFilePathBox_Image;
	private: System::Windows::Forms::PictureBox^ QuickLootFilePathBox_Image;
	private: System::Windows::Forms::PictureBox^ ConstructibleObjectMenuFilePathBox_Image;
	private: System::Windows::Forms::PictureBox^ OutputFilesBox_Image;
	private: System::Text::StringBuilder^ logBuilder;
	private: System::ComponentModel::IContainer^ components;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::ToolTip^ toolTip1;

	private: ComponentFactory::Krypton::Toolkit::KryptonButton^ About;
	public: System::Windows::Forms::CheckBox^ CreateNewModFolder;


	public:
		static String^ CurrentFile;
		static bool modManagerLaunched;

		MyForm(array<String^>^ args)
		{
			InitializeComponent();
			logBuilder = gcnew StringBuilder();
			instance = this;

			HandleCommandLineArguments(args);

			if (modManagerLaunched) {
				this->LoadFilePathsFromMO2();
				this->CreateNewModFolder->Visible = true;
			}
			else
			{
				this->CreateNewModFolder->Visible = false;
			}
		}

		static MyForm^ GetInstance() { return instance; };
		static std::string toStdString(String^ managedString) {
			return msclr::interop::marshal_as<std::string>(managedString);
		}

		void LoadFilePathsFromMO2()
		{
			this->ClearLog();
			this->LogMessage("Attempting to auto load file paths...");

			String^ exePath = System::Windows::Forms::Application::StartupPath;

			// Combine with "interface" folder to check
			String^ interfaceFolderPath = Path::Combine(exePath, "interface");

			if (!Directory::Exists(interfaceFolderPath)) {
				Directory::CreateDirectory(interfaceFolderPath);  // Create the directory if it doesn't exist
			}

			// Combine with "skyui" folder and file name
			String^ skyuiFolderPath = Path::Combine(interfaceFolderPath, "skyui");
			String^ inventorylistsFilePath = Path::Combine(skyuiFolderPath, "inventorylists.swf");
			String^ craftingmenuFilePath = Path::Combine(interfaceFolderPath, "craftingmenu.swf");
			String^ cocksmenuFilePath = Path::Combine(interfaceFolderPath, "constructibleobjectmenu.swf");
			String^ lootmenuFilePath = Path::Combine(interfaceFolderPath, "lootmenu.swf");

			// Check if the "inventorylists.swf" file exists within "skyui" folder
			if (File::Exists(inventorylistsFilePath)) {
				this->InventoryListsFilePathBox->Text = inventorylistsFilePath;
			}
			else
			{
				this->LogMessage("Unable to find inventorylists.swf");
			}

			// Check if the "craftingmenu.swf" file exists within "interface" folder
			if (File::Exists(craftingmenuFilePath)) {
				this->CraftingMenuFilePathBox->Text = craftingmenuFilePath;
			}
			else
			{
				this->LogMessage("Unable to find craftingmenu.swf");
			}

			// Check if the "constructibleobjectmenu.swf" file exists within "interface" folder
			if (File::Exists(cocksmenuFilePath)) {
				this->ConstructibleObjectMenuFilePathBox->Text = cocksmenuFilePath;
			}
			else
			{
				this->LogMessage("Unable to find constructibleobjectmenu.swf");
			}

			// Check if the "lootmenu.swf" file exists within "interface" folder
			if (File::Exists(lootmenuFilePath)) {
				if (!File::Exists("QuickLootIE.esp")) {
					this->QuickLootFilePathBox->Text = lootmenuFilePath;
				}
				else
				{
					this->LogMessage("QuicklootIE detected, lootmenu.swf will not be patched...");
				}
			}
			else
			{
				this->LogMessage("Unable to find lootmenu.swf");
			}
		}

		void LogMessage(const std::string& message)
		{
			// Get the current date and time
			time_t now;
			time(&now);

			struct tm ltm;
			localtime_s(&ltm, &now);

			// Format date and time in British format
			std::stringstream dateTimeStream;
			dateTimeStream << (ltm.tm_mday < 10 ? "0" : "") << ltm.tm_mday << "/"
				<< (1 + ltm.tm_mon < 10 ? "0" : "") << (1 + ltm.tm_mon) << "/"
				<< 1900 + ltm.tm_year << " "
				<< (ltm.tm_hour < 10 ? "0" : "") << ltm.tm_hour << ":"
				<< (ltm.tm_min < 10 ? "0" : "") << ltm.tm_min << ":"
				<< (ltm.tm_sec < 10 ? "0" : "") << ltm.tm_sec;

			std::string dateTimeString = dateTimeStream.str();

			// Prepend the date and time to the message
			std::string logMessage = "[" + dateTimeString + "] " + message;

			// Convert the log message to a managed string and append it to the log
			String^ managedMessage = msclr::interop::marshal_as<String^>(logMessage);
			logBuilder->AppendLine(managedMessage);
			log->Text = logBuilder->ToString();

			log->SelectionStart = log->Text->Length;
			log->ScrollToCaret();
			Application::DoEvents();
		}

		void ClearLog()
		{
			logBuilder->Clear();
			log->Text = logBuilder->ToString();
		}

		void ResetLog()
		{
			logBuilder->Clear();
			log->Text = "Execution log...";
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		void HandleCommandLineArguments(array<String^>^ args)
		{
			// Process the command-line arguments
			for each (String ^ arg in args)
			{
				if (arg == "-mo2") {
					if (standard::interfaceFolderExists()) {
						modManagerLaunched = true;
					}
					else
					{
						this->LogMessage("Unable to locate interface folder...");
					}
					
				}
			}
		}

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->kryptonPalette1 = (gcnew ComponentFactory::Krypton::Toolkit::KryptonPalette(this->components));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->InventoryListsFilePathBox = (gcnew ComponentFactory::Krypton::Toolkit::KryptonTextBox());
			this->RunButton = (gcnew ComponentFactory::Krypton::Toolkit::KryptonButton());
			this->CraftingMenuFilePathBox = (gcnew ComponentFactory::Krypton::Toolkit::KryptonTextBox());
			this->QuickLootFilePathBox = (gcnew ComponentFactory::Krypton::Toolkit::KryptonTextBox());
			this->ConstructibleObjectMenuFilePathBox = (gcnew ComponentFactory::Krypton::Toolkit::KryptonTextBox());
			this->CopyLog = (gcnew ComponentFactory::Krypton::Toolkit::KryptonButton());
			this->log = (gcnew ComponentFactory::Krypton::Toolkit::KryptonRichTextBox());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->OutputFilesBox = (gcnew ComponentFactory::Krypton::Toolkit::KryptonTextBox());
			this->InventoryListsFilePathBox_Image = (gcnew System::Windows::Forms::PictureBox());
			this->CraftingMenuFilePathBox_Image = (gcnew System::Windows::Forms::PictureBox());
			this->QuickLootFilePathBox_Image = (gcnew System::Windows::Forms::PictureBox());
			this->ConstructibleObjectMenuFilePathBox_Image = (gcnew System::Windows::Forms::PictureBox());
			this->OutputFilesBox_Image = (gcnew System::Windows::Forms::PictureBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->CreateNewModFolder = (gcnew System::Windows::Forms::CheckBox());
			this->About = (gcnew ComponentFactory::Krypton::Toolkit::KryptonButton());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->InventoryListsFilePathBox_Image))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CraftingMenuFilePathBox_Image))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->QuickLootFilePathBox_Image))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ConstructibleObjectMenuFilePathBox_Image))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->OutputFilesBox_Image))->BeginInit();
			this->SuspendLayout();
			// 
			// kryptonPalette1
			// 
			this->kryptonPalette1->ButtonStyles->ButtonForm->StateNormal->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->ButtonStyles->ButtonForm->StateNormal->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->ButtonStyles->ButtonForm->StateNormal->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->kryptonPalette1->ButtonStyles->ButtonForm->StateNormal->Border->Width = 0;
			this->kryptonPalette1->ButtonStyles->ButtonForm->StatePressed->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->ButtonStyles->ButtonForm->StatePressed->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->ButtonStyles->ButtonForm->StatePressed->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->kryptonPalette1->ButtonStyles->ButtonForm->StatePressed->Border->Width = 0;
			this->kryptonPalette1->ButtonStyles->ButtonForm->StateTracking->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->ButtonStyles->ButtonForm->StateTracking->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->ButtonStyles->ButtonForm->StateTracking->Back->ColorAlign = ComponentFactory::Krypton::Toolkit::PaletteRectangleAlign::Control;
			this->kryptonPalette1->ButtonStyles->ButtonForm->StateTracking->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->kryptonPalette1->ButtonStyles->ButtonForm->StateTracking->Border->Width = 0;
			this->kryptonPalette1->ButtonStyles->ButtonFormClose->StatePressed->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->ButtonStyles->ButtonFormClose->StatePressed->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->ButtonStyles->ButtonFormClose->StatePressed->Back->ColorAlign = ComponentFactory::Krypton::Toolkit::PaletteRectangleAlign::Control;
			this->kryptonPalette1->ButtonStyles->ButtonFormClose->StatePressed->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->kryptonPalette1->ButtonStyles->ButtonFormClose->StatePressed->Border->Width = 0;
			this->kryptonPalette1->ButtonStyles->ButtonFormClose->StateTracking->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->ButtonStyles->ButtonFormClose->StateTracking->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->ButtonStyles->ButtonFormClose->StateTracking->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->kryptonPalette1->ButtonStyles->ButtonFormClose->StateTracking->Border->Width = 0;
			this->kryptonPalette1->FormStyles->FormMain->StateCommon->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->FormStyles->FormMain->StateCommon->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->FormStyles->FormMain->StateCommon->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->FormStyles->FormMain->StateCommon->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->kryptonPalette1->FormStyles->FormMain->StateCommon->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::None;
			this->kryptonPalette1->FormStyles->FormMain->StateCommon->Border->Rounding = 12;
			this->kryptonPalette1->HeaderStyles->HeaderForm->StateCommon->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->HeaderStyles->HeaderForm->StateCommon->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->kryptonPalette1->HeaderStyles->HeaderForm->StateCommon->ButtonEdgeInset = 10;
			this->kryptonPalette1->HeaderStyles->HeaderForm->StateCommon->Content->Padding = System::Windows::Forms::Padding(10, -1, -1, -1);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display Semib", 15, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(687, 53);
			this->label1->TabIndex = 11;
			this->label1->Text = L"Completionist Automated UI Patcher";
			// 
			// InventoryListsFilePathBox
			// 
			this->InventoryListsFilePathBox->Location = System::Drawing::Point(21, 217);
			this->InventoryListsFilePathBox->Name = L"InventoryListsFilePathBox";
			this->InventoryListsFilePathBox->ReadOnly = true;
			this->InventoryListsFilePathBox->Size = System::Drawing::Size(678, 42);
			this->InventoryListsFilePathBox->StateCommon->Back->Color1 = System::Drawing::Color::White;
			this->InventoryListsFilePathBox->StateCommon->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->InventoryListsFilePathBox->StateCommon->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->InventoryListsFilePathBox->StateCommon->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->InventoryListsFilePathBox->StateCommon->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->InventoryListsFilePathBox->StateCommon->Border->Rounding = 20;
			this->InventoryListsFilePathBox->StateCommon->Border->Width = 1;
			this->InventoryListsFilePathBox->StateCommon->Content->Color1 = System::Drawing::Color::Gray;
			this->InventoryListsFilePathBox->StateCommon->Content->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display", 7.875F,
				System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->InventoryListsFilePathBox->StateCommon->Content->Padding = System::Windows::Forms::Padding(10, 0, 10, 0);
			this->InventoryListsFilePathBox->TabIndex = 13;
			this->InventoryListsFilePathBox->TabStop = false;
			this->InventoryListsFilePathBox->Text = L"Select \'InventoryLists.swf\'";
			// 
			// RunButton
			// 
			this->RunButton->Location = System::Drawing::Point(21, 409);
			this->RunButton->Name = L"RunButton";
			this->RunButton->OverrideDefault->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(6)),
				static_cast<System::Int32>(static_cast<System::Byte>(174)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->RunButton->OverrideDefault->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(6)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->RunButton->OverrideDefault->Back->ColorAngle = 45;
			this->RunButton->OverrideDefault->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(6)),
				static_cast<System::Int32>(static_cast<System::Byte>(174)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->RunButton->OverrideDefault->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->RunButton->OverrideDefault->Border->ColorAngle = 45;
			this->RunButton->OverrideDefault->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->RunButton->OverrideDefault->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->RunButton->OverrideDefault->Border->Rounding = 20;
			this->RunButton->OverrideDefault->Border->Width = 1;
			this->RunButton->PaletteMode = ComponentFactory::Krypton::Toolkit::PaletteMode::ProfessionalSystem;
			this->RunButton->Size = System::Drawing::Size(209, 51);
			this->RunButton->StateCommon->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(6)),
				static_cast<System::Int32>(static_cast<System::Byte>(174)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->RunButton->StateCommon->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->RunButton->StateCommon->Back->ColorAngle = 45;
			this->RunButton->StateCommon->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(6)),
				static_cast<System::Int32>(static_cast<System::Byte>(174)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->RunButton->StateCommon->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->RunButton->StateCommon->Border->ColorAngle = 45;
			this->RunButton->StateCommon->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->RunButton->StateCommon->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->RunButton->StateCommon->Border->Rounding = 20;
			this->RunButton->StateCommon->Border->Width = 1;
			this->RunButton->StateCommon->Content->ShortText->Color1 = System::Drawing::Color::White;
			this->RunButton->StateCommon->Content->ShortText->Color2 = System::Drawing::Color::White;
			this->RunButton->StateCommon->Content->ShortText->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display", 7.875F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->RunButton->StatePressed->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(20)),
				static_cast<System::Int32>(static_cast<System::Byte>(145)), static_cast<System::Int32>(static_cast<System::Byte>(198)));
			this->RunButton->StatePressed->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(22)),
				static_cast<System::Int32>(static_cast<System::Byte>(121)), static_cast<System::Int32>(static_cast<System::Byte>(206)));
			this->RunButton->StatePressed->Back->ColorAngle = 135;
			this->RunButton->StatePressed->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(20)),
				static_cast<System::Int32>(static_cast<System::Byte>(145)), static_cast<System::Int32>(static_cast<System::Byte>(198)));
			this->RunButton->StatePressed->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(22)),
				static_cast<System::Int32>(static_cast<System::Byte>(121)), static_cast<System::Int32>(static_cast<System::Byte>(206)));
			this->RunButton->StatePressed->Border->ColorAngle = 135;
			this->RunButton->StatePressed->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->RunButton->StatePressed->Border->Rounding = 20;
			this->RunButton->StatePressed->Border->Width = 1;
			this->RunButton->StateTracking->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(152)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->RunButton->StateTracking->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(6)),
				static_cast<System::Int32>(static_cast<System::Byte>(174)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->RunButton->StateTracking->Back->ColorAngle = 45;
			this->RunButton->StateTracking->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(6)),
				static_cast<System::Int32>(static_cast<System::Byte>(174)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->RunButton->StateTracking->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->RunButton->StateTracking->Border->ColorAngle = 45;
			this->RunButton->StateTracking->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->RunButton->StateTracking->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->RunButton->StateTracking->Border->Rounding = 20;
			this->RunButton->StateTracking->Border->Width = 1;
			this->RunButton->TabIndex = 14;
			this->RunButton->Values->Text = L"Start Patching";
			this->RunButton->Click += gcnew System::EventHandler(this, &MyForm::RunButton_Click);
			// 
			// CraftingMenuFilePathBox
			// 
			this->CraftingMenuFilePathBox->Location = System::Drawing::Point(21, 265);
			this->CraftingMenuFilePathBox->Name = L"CraftingMenuFilePathBox";
			this->CraftingMenuFilePathBox->Size = System::Drawing::Size(678, 42);
			this->CraftingMenuFilePathBox->StateCommon->Back->Color1 = System::Drawing::Color::White;
			this->CraftingMenuFilePathBox->StateCommon->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->CraftingMenuFilePathBox->StateCommon->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->CraftingMenuFilePathBox->StateCommon->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->CraftingMenuFilePathBox->StateCommon->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->CraftingMenuFilePathBox->StateCommon->Border->Rounding = 20;
			this->CraftingMenuFilePathBox->StateCommon->Border->Width = 1;
			this->CraftingMenuFilePathBox->StateCommon->Content->Color1 = System::Drawing::Color::Gray;
			this->CraftingMenuFilePathBox->StateCommon->Content->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display", 7.875F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->CraftingMenuFilePathBox->StateCommon->Content->Padding = System::Windows::Forms::Padding(10, 0, 10, 0);
			this->CraftingMenuFilePathBox->TabIndex = 15;
			this->CraftingMenuFilePathBox->TabStop = false;
			this->CraftingMenuFilePathBox->Text = L"Select \'CraftingMenu.swf\'";
			// 
			// QuickLootFilePathBox
			// 
			this->QuickLootFilePathBox->Location = System::Drawing::Point(21, 313);
			this->QuickLootFilePathBox->Name = L"QuickLootFilePathBox";
			this->QuickLootFilePathBox->Size = System::Drawing::Size(678, 42);
			this->QuickLootFilePathBox->StateCommon->Back->Color1 = System::Drawing::Color::White;
			this->QuickLootFilePathBox->StateCommon->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->QuickLootFilePathBox->StateCommon->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->QuickLootFilePathBox->StateCommon->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->QuickLootFilePathBox->StateCommon->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->QuickLootFilePathBox->StateCommon->Border->Rounding = 20;
			this->QuickLootFilePathBox->StateCommon->Border->Width = 1;
			this->QuickLootFilePathBox->StateCommon->Content->Color1 = System::Drawing::Color::Gray;
			this->QuickLootFilePathBox->StateCommon->Content->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display", 7.875F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->QuickLootFilePathBox->StateCommon->Content->Padding = System::Windows::Forms::Padding(10, 0, 10, 0);
			this->QuickLootFilePathBox->TabIndex = 16;
			this->QuickLootFilePathBox->TabStop = false;
			this->QuickLootFilePathBox->Text = L"Select \'LootMenu.swf\'";
			// 
			// ConstructibleObjectMenuFilePathBox
			// 
			this->ConstructibleObjectMenuFilePathBox->Location = System::Drawing::Point(21, 361);
			this->ConstructibleObjectMenuFilePathBox->Name = L"ConstructibleObjectMenuFilePathBox";
			this->ConstructibleObjectMenuFilePathBox->Size = System::Drawing::Size(678, 42);
			this->ConstructibleObjectMenuFilePathBox->StateCommon->Back->Color1 = System::Drawing::Color::White;
			this->ConstructibleObjectMenuFilePathBox->StateCommon->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->ConstructibleObjectMenuFilePathBox->StateCommon->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->ConstructibleObjectMenuFilePathBox->StateCommon->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->ConstructibleObjectMenuFilePathBox->StateCommon->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->ConstructibleObjectMenuFilePathBox->StateCommon->Border->Rounding = 20;
			this->ConstructibleObjectMenuFilePathBox->StateCommon->Border->Width = 1;
			this->ConstructibleObjectMenuFilePathBox->StateCommon->Content->Color1 = System::Drawing::Color::Gray;
			this->ConstructibleObjectMenuFilePathBox->StateCommon->Content->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display",
				7.875F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->ConstructibleObjectMenuFilePathBox->StateCommon->Content->Padding = System::Windows::Forms::Padding(10, 0, 10, 0);
			this->ConstructibleObjectMenuFilePathBox->TabIndex = 17;
			this->ConstructibleObjectMenuFilePathBox->TabStop = false;
			this->ConstructibleObjectMenuFilePathBox->Text = L"Select \'ConstructibleObjectMenu.swf\'";
			// 
			// CopyLog
			// 
			this->CopyLog->Location = System::Drawing::Point(1300, 409);
			this->CopyLog->Name = L"CopyLog";
			this->CopyLog->OverrideDefault->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->CopyLog->OverrideDefault->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->CopyLog->OverrideDefault->Back->ColorAngle = 45;
			this->CopyLog->OverrideDefault->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->CopyLog->OverrideDefault->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->CopyLog->OverrideDefault->Border->ColorAngle = 45;
			this->CopyLog->OverrideDefault->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->CopyLog->OverrideDefault->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->CopyLog->OverrideDefault->Border->Rounding = 20;
			this->CopyLog->OverrideDefault->Border->Width = 1;
			this->CopyLog->PaletteMode = ComponentFactory::Krypton::Toolkit::PaletteMode::ProfessionalSystem;
			this->CopyLog->Size = System::Drawing::Size(209, 51);
			this->CopyLog->StateCommon->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->CopyLog->StateCommon->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->CopyLog->StateCommon->Back->ColorAngle = 45;
			this->CopyLog->StateCommon->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(6)),
				static_cast<System::Int32>(static_cast<System::Byte>(174)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CopyLog->StateCommon->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->CopyLog->StateCommon->Border->ColorAngle = 45;
			this->CopyLog->StateCommon->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->CopyLog->StateCommon->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->CopyLog->StateCommon->Border->Rounding = 20;
			this->CopyLog->StateCommon->Border->Width = 1;
			this->CopyLog->StateCommon->Content->ShortText->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->CopyLog->StateCommon->Content->ShortText->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display", 7.875F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->CopyLog->StatePressed->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(20)),
				static_cast<System::Int32>(static_cast<System::Byte>(145)), static_cast<System::Int32>(static_cast<System::Byte>(198)));
			this->CopyLog->StatePressed->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(22)),
				static_cast<System::Int32>(static_cast<System::Byte>(121)), static_cast<System::Int32>(static_cast<System::Byte>(206)));
			this->CopyLog->StatePressed->Back->ColorAngle = 135;
			this->CopyLog->StatePressed->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(20)),
				static_cast<System::Int32>(static_cast<System::Byte>(145)), static_cast<System::Int32>(static_cast<System::Byte>(198)));
			this->CopyLog->StatePressed->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(22)),
				static_cast<System::Int32>(static_cast<System::Byte>(121)), static_cast<System::Int32>(static_cast<System::Byte>(206)));
			this->CopyLog->StatePressed->Border->ColorAngle = 135;
			this->CopyLog->StatePressed->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->CopyLog->StatePressed->Border->Rounding = 20;
			this->CopyLog->StatePressed->Border->Width = 1;
			this->CopyLog->StatePressed->Content->ShortText->Color1 = System::Drawing::Color::White;
			this->CopyLog->StateTracking->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(152)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->CopyLog->StateTracking->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(6)),
				static_cast<System::Int32>(static_cast<System::Byte>(174)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CopyLog->StateTracking->Back->ColorAngle = 45;
			this->CopyLog->StateTracking->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(6)),
				static_cast<System::Int32>(static_cast<System::Byte>(174)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CopyLog->StateTracking->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->CopyLog->StateTracking->Border->ColorAngle = 45;
			this->CopyLog->StateTracking->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->CopyLog->StateTracking->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->CopyLog->StateTracking->Border->Rounding = 20;
			this->CopyLog->StateTracking->Border->Width = 1;
			this->CopyLog->StateTracking->Content->ShortText->Color1 = System::Drawing::Color::White;
			this->CopyLog->TabIndex = 19;
			this->CopyLog->Values->Text = L"Copy Log";
			this->CopyLog->Click += gcnew System::EventHandler(this, &MyForm::CopyLog_Click);
			// 
			// log
			// 
			this->log->Location = System::Drawing::Point(745, 217);
			this->log->Name = L"log";
			this->log->ReadOnly = true;
			this->log->Size = System::Drawing::Size(764, 186);
			this->log->StateCommon->Back->Color1 = System::Drawing::Color::White;
			this->log->StateCommon->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->log->StateCommon->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->log->StateCommon->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->log->StateCommon->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->log->StateCommon->Border->Rounding = 20;
			this->log->StateCommon->Border->Width = 1;
			this->log->StateCommon->Content->Color1 = System::Drawing::Color::Gray;
			this->log->StateCommon->Content->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display", 7.875F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->log->StateCommon->Content->Padding = System::Windows::Forms::Padding(10, 0, 10, 0);
			this->log->TabIndex = 20;
			this->log->Text = L"Execution log...";
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(583, -18);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(764, 234);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox1->TabIndex = 21;
			this->pictureBox1->TabStop = false;
			// 
			// OutputFilesBox
			// 
			this->OutputFilesBox->Location = System::Drawing::Point(745, 418);
			this->OutputFilesBox->Name = L"OutputFilesBox";
			this->OutputFilesBox->ReadOnly = true;
			this->OutputFilesBox->Size = System::Drawing::Size(533, 42);
			this->OutputFilesBox->StateCommon->Back->Color1 = System::Drawing::Color::White;
			this->OutputFilesBox->StateCommon->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->OutputFilesBox->StateCommon->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->OutputFilesBox->StateCommon->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->OutputFilesBox->StateCommon->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->OutputFilesBox->StateCommon->Border->Rounding = 20;
			this->OutputFilesBox->StateCommon->Border->Width = 1;
			this->OutputFilesBox->StateCommon->Content->Color1 = System::Drawing::Color::Gray;
			this->OutputFilesBox->StateCommon->Content->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display", 7.875F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->OutputFilesBox->StateCommon->Content->Padding = System::Windows::Forms::Padding(10, 0, 10, 0);
			this->OutputFilesBox->TabIndex = 22;
			this->OutputFilesBox->TabStop = false;
			this->OutputFilesBox->Text = L"{Current Directory}/output";
			// 
			// InventoryListsFilePathBox_Image
			// 
			this->InventoryListsFilePathBox_Image->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->InventoryListsFilePathBox_Image->BackColor = System::Drawing::Color::White;
			this->InventoryListsFilePathBox_Image->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->InventoryListsFilePathBox_Image->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"InventoryListsFilePathBox_Image.Image")));
			this->InventoryListsFilePathBox_Image->Location = System::Drawing::Point(653, 222);
			this->InventoryListsFilePathBox_Image->Name = L"InventoryListsFilePathBox_Image";
			this->InventoryListsFilePathBox_Image->Size = System::Drawing::Size(32, 32);
			this->InventoryListsFilePathBox_Image->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->InventoryListsFilePathBox_Image->TabIndex = 24;
			this->InventoryListsFilePathBox_Image->TabStop = false;
			this->InventoryListsFilePathBox_Image->Click += gcnew System::EventHandler(this, &MyForm::InventoryListsFilePathBox_Image_Click);
			// 
			// CraftingMenuFilePathBox_Image
			// 
			this->CraftingMenuFilePathBox_Image->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->CraftingMenuFilePathBox_Image->BackColor = System::Drawing::Color::White;
			this->CraftingMenuFilePathBox_Image->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->CraftingMenuFilePathBox_Image->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"CraftingMenuFilePathBox_Image.Image")));
			this->CraftingMenuFilePathBox_Image->Location = System::Drawing::Point(653, 267);
			this->CraftingMenuFilePathBox_Image->Name = L"CraftingMenuFilePathBox_Image";
			this->CraftingMenuFilePathBox_Image->Size = System::Drawing::Size(32, 32);
			this->CraftingMenuFilePathBox_Image->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->CraftingMenuFilePathBox_Image->TabIndex = 25;
			this->CraftingMenuFilePathBox_Image->TabStop = false;
			this->CraftingMenuFilePathBox_Image->Click += gcnew System::EventHandler(this, &MyForm::CraftingMenuFilePathBox_Image_Click);
			// 
			// QuickLootFilePathBox_Image
			// 
			this->QuickLootFilePathBox_Image->BackColor = System::Drawing::Color::White;
			this->QuickLootFilePathBox_Image->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->QuickLootFilePathBox_Image->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"QuickLootFilePathBox_Image.Image")));
			this->QuickLootFilePathBox_Image->Location = System::Drawing::Point(653, 315);
			this->QuickLootFilePathBox_Image->Name = L"QuickLootFilePathBox_Image";
			this->QuickLootFilePathBox_Image->Size = System::Drawing::Size(32, 32);
			this->QuickLootFilePathBox_Image->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->QuickLootFilePathBox_Image->TabIndex = 26;
			this->QuickLootFilePathBox_Image->TabStop = false;
			this->toolTip1->SetToolTip(this->QuickLootFilePathBox_Image, L"Only for QuicklootEE and QuicklootRE - QuicklootIE has direct API Support.");
			this->QuickLootFilePathBox_Image->Click += gcnew System::EventHandler(this, &MyForm::QuickLootFilePathBox_Image_Click);
			// 
			// ConstructibleObjectMenuFilePathBox_Image
			// 
			this->ConstructibleObjectMenuFilePathBox_Image->Anchor = System::Windows::Forms::AnchorStyles::Top;
			this->ConstructibleObjectMenuFilePathBox_Image->BackColor = System::Drawing::Color::White;
			this->ConstructibleObjectMenuFilePathBox_Image->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->ConstructibleObjectMenuFilePathBox_Image->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ConstructibleObjectMenuFilePathBox_Image.Image")));
			this->ConstructibleObjectMenuFilePathBox_Image->Location = System::Drawing::Point(653, 365);
			this->ConstructibleObjectMenuFilePathBox_Image->Name = L"ConstructibleObjectMenuFilePathBox_Image";
			this->ConstructibleObjectMenuFilePathBox_Image->Size = System::Drawing::Size(32, 32);
			this->ConstructibleObjectMenuFilePathBox_Image->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
			this->ConstructibleObjectMenuFilePathBox_Image->TabIndex = 27;
			this->ConstructibleObjectMenuFilePathBox_Image->TabStop = false;
			this->ConstructibleObjectMenuFilePathBox_Image->Click += gcnew System::EventHandler(this, &MyForm::ConstructibleObjectMenuFilePathBox_Image_Click);
			// 
			// OutputFilesBox_Image
			// 
			this->OutputFilesBox_Image->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->OutputFilesBox_Image->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"OutputFilesBox_Image.Image")));
			this->OutputFilesBox_Image->Location = System::Drawing::Point(1223, 423);
			this->OutputFilesBox_Image->Name = L"OutputFilesBox_Image";
			this->OutputFilesBox_Image->Size = System::Drawing::Size(32, 32);
			this->OutputFilesBox_Image->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->OutputFilesBox_Image->TabIndex = 28;
			this->OutputFilesBox_Image->TabStop = false;
			this->toolTip1->SetToolTip(this->OutputFilesBox_Image, L"Select an ouput directory where the patcher will save the compiled SWF\'s");
			this->OutputFilesBox_Image->Click += gcnew System::EventHandler(this, &MyForm::OutputFilesBox_Image_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display", 7.875F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label3->Location = System::Drawing::Point(16, 62);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(128, 28);
			this->label3->TabIndex = 29;
			this->label3->Text = L"Version: 1.0.5";
			// 
			// toolTip1
			// 
			this->toolTip1->AutomaticDelay = 0;
			this->toolTip1->AutoPopDelay = 0;
			this->toolTip1->InitialDelay = 0;
			this->toolTip1->ReshowDelay = 100;
			// 
			// CreateNewModFolder
			// 
			this->CreateNewModFolder->AutoSize = true;
			this->CreateNewModFolder->ForeColor = System::Drawing::Color::Gray;
			this->CreateNewModFolder->Location = System::Drawing::Point(372, 423);
			this->CreateNewModFolder->Name = L"CreateNewModFolder";
			this->CreateNewModFolder->Size = System::Drawing::Size(327, 32);
			this->CreateNewModFolder->TabIndex = 32;
			this->CreateNewModFolder->Text = L"Create New Mod Folder (MO2)";
			this->toolTip1->SetToolTip(this->CreateNewModFolder, L"\r\n");
			this->CreateNewModFolder->UseVisualStyleBackColor = true;
			this->CreateNewModFolder->CheckedChanged += gcnew System::EventHandler(this, &MyForm::CreateNewModFolder_CheckedChanged);
			// 
			// About
			// 
			this->About->Location = System::Drawing::Point(1387, 1);
			this->About->Name = L"About";
			this->About->OverrideDefault->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->About->OverrideDefault->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->About->OverrideDefault->Back->ColorAngle = 45;
			this->About->OverrideDefault->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->About->OverrideDefault->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->About->OverrideDefault->Border->ColorAngle = 45;
			this->About->OverrideDefault->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->About->OverrideDefault->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->About->OverrideDefault->Border->Rounding = 20;
			this->About->OverrideDefault->Border->Width = 1;
			this->About->PaletteMode = ComponentFactory::Krypton::Toolkit::PaletteMode::ProfessionalSystem;
			this->About->Size = System::Drawing::Size(122, 48);
			this->About->StateCommon->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->About->StateCommon->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->About->StateCommon->Back->ColorAngle = 45;
			this->About->StateCommon->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(6)),
				static_cast<System::Int32>(static_cast<System::Byte>(174)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->About->StateCommon->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->About->StateCommon->Border->ColorAngle = 45;
			this->About->StateCommon->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->About->StateCommon->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->About->StateCommon->Border->Rounding = 20;
			this->About->StateCommon->Border->Width = 1;
			this->About->StateCommon->Content->ShortText->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->About->StateCommon->Content->ShortText->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display", 7.125F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->About->StatePressed->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(20)),
				static_cast<System::Int32>(static_cast<System::Byte>(145)), static_cast<System::Int32>(static_cast<System::Byte>(198)));
			this->About->StatePressed->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(22)),
				static_cast<System::Int32>(static_cast<System::Byte>(121)), static_cast<System::Int32>(static_cast<System::Byte>(206)));
			this->About->StatePressed->Back->ColorAngle = 135;
			this->About->StatePressed->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(20)),
				static_cast<System::Int32>(static_cast<System::Byte>(145)), static_cast<System::Int32>(static_cast<System::Byte>(198)));
			this->About->StatePressed->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(22)),
				static_cast<System::Int32>(static_cast<System::Byte>(121)), static_cast<System::Int32>(static_cast<System::Byte>(206)));
			this->About->StatePressed->Border->ColorAngle = 135;
			this->About->StatePressed->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->About->StatePressed->Border->Rounding = 20;
			this->About->StatePressed->Border->Width = 1;
			this->About->StatePressed->Content->ShortText->Color1 = System::Drawing::Color::White;
			this->About->StateTracking->Back->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(152)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->About->StateTracking->Back->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(6)),
				static_cast<System::Int32>(static_cast<System::Byte>(174)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->About->StateTracking->Back->ColorAngle = 45;
			this->About->StateTracking->Border->Color1 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(6)),
				static_cast<System::Int32>(static_cast<System::Byte>(174)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->About->StateTracking->Border->Color2 = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(8)),
				static_cast<System::Int32>(static_cast<System::Byte>(142)), static_cast<System::Int32>(static_cast<System::Byte>(254)));
			this->About->StateTracking->Border->ColorAngle = 45;
			this->About->StateTracking->Border->DrawBorders = static_cast<ComponentFactory::Krypton::Toolkit::PaletteDrawBorders>((((ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Top | ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Bottom)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Left)
				| ComponentFactory::Krypton::Toolkit::PaletteDrawBorders::Right));
			this->About->StateTracking->Border->GraphicsHint = ComponentFactory::Krypton::Toolkit::PaletteGraphicsHint::AntiAlias;
			this->About->StateTracking->Border->Rounding = 20;
			this->About->StateTracking->Border->Width = 1;
			this->About->StateTracking->Content->ShortText->Color1 = System::Drawing::Color::White;
			this->About->TabIndex = 31;
			this->About->Values->Text = L"About";
			this->About->Click += gcnew System::EventHandler(this, &MyForm::About_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(12, 28);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)), static_cast<System::Int32>(static_cast<System::Byte>(252)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(1521, 461);
			this->Controls->Add(this->CreateNewModFolder);
			this->Controls->Add(this->About);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->OutputFilesBox_Image);
			this->Controls->Add(this->ConstructibleObjectMenuFilePathBox_Image);
			this->Controls->Add(this->QuickLootFilePathBox_Image);
			this->Controls->Add(this->CraftingMenuFilePathBox_Image);
			this->Controls->Add(this->InventoryListsFilePathBox_Image);
			this->Controls->Add(this->OutputFilesBox);
			this->Controls->Add(this->log);
			this->Controls->Add(this->CopyLog);
			this->Controls->Add(this->ConstructibleObjectMenuFilePathBox);
			this->Controls->Add(this->QuickLootFilePathBox);
			this->Controls->Add(this->CraftingMenuFilePathBox);
			this->Controls->Add(this->RunButton);
			this->Controls->Add(this->InventoryListsFilePathBox);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox1);
			this->Font = (gcnew System::Drawing::Font(L"Segoe UI Variable Display", 7.875F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ForeColor = System::Drawing::Color::White;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Location = System::Drawing::Point(32, 91);
			this->MaximizeBox = false;
			this->Name = L"MyForm";
			this->Palette = this->kryptonPalette1;
			this->PaletteMode = ComponentFactory::Krypton::Toolkit::PaletteMode::Custom;
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Completionist Automated UI Patcher...";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->InventoryListsFilePathBox_Image))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CraftingMenuFilePathBox_Image))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->QuickLootFilePathBox_Image))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ConstructibleObjectMenuFilePathBox_Image))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->OutputFilesBox_Image))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void InventoryListsFilePathBox_Image_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
		openFileDialog->Filter = "SWF Files (*.swf)|*.swf";

		while (true) {
			if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				String^ selectedFile = openFileDialog->FileName;
				String^ expectedFileName = "inventorylists.swf";

				if (Path::GetFileName(selectedFile)->Equals(expectedFileName, StringComparison::OrdinalIgnoreCase)) {
					InventoryListsFilePathBox->Text = selectedFile;
					break; // Exit the loop if the correct file is selected
				}
				else {
					MessageBox::Show("Please select the inventorylists.swf file.", "Invalid File", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
			}
			else {
				break; // Exit the loop if the dialog is canceled
			}
		}
	}

	private: System::Void CraftingMenuFilePathBox_Image_Click(System::Object^ sender, System::EventArgs^ e)
	{
		OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
		openFileDialog->Filter = "SWF Files (*.swf)|*.swf";

		while (true) {
			if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				String^ selectedFile = openFileDialog->FileName;
				String^ expectedFileName = "craftingmenu.swf";

				if (Path::GetFileName(selectedFile)->Equals(expectedFileName, StringComparison::OrdinalIgnoreCase)) {
					CraftingMenuFilePathBox->Text = selectedFile;
					break; // Exit the loop if the correct file is selected
				}
				else {
					MessageBox::Show("Please select the craftingmenu.swf file.", "Invalid File", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
			}
			else {
				break; // Exit the loop if the dialog is canceled
			}
		}
	}

	private: System::Void ConstructibleObjectMenuFilePathBox_Image_Click(System::Object^ sender, System::EventArgs^ e)
	{
		OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
		openFileDialog->Filter = "SWF Files (*.swf)|*.swf";

		while (true) {
			if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				String^ selectedFile = openFileDialog->FileName;
				String^ expectedFileName = "constructibleobjectmenu.swf";

				if (Path::GetFileName(selectedFile)->Equals(expectedFileName, StringComparison::OrdinalIgnoreCase)) {
					ConstructibleObjectMenuFilePathBox->Text = selectedFile;
					break; // Exit the loop if the correct file is selected
				}
				else {
					MessageBox::Show("Please select the constructibleobjectmenu.swf file.", "Invalid File", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
			}
			else {
				break; // Exit the loop if the dialog is canceled
			}
		}
	}

	private: System::Void QuickLootFilePathBox_Image_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (File::Exists("QuickLootIE.esp")) {
			return;
		}

		OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
		openFileDialog->Filter = "SWF Files (*.swf)|*.swf";

		while (true) {
			if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				String^ selectedFile = openFileDialog->FileName;
				String^ expectedFileName = "lootmenu.swf";

				if (Path::GetFileName(selectedFile)->Equals(expectedFileName, StringComparison::OrdinalIgnoreCase)) {
					QuickLootFilePathBox->Text = selectedFile;
					break; // Exit the loop if the correct file is selected
				}
				else {
					MessageBox::Show("Please select the lootmenu.swf file.", "Invalid File", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
			}
			else {
				break; // Exit the loop if the dialog is canceled
			}
		}
	}

	private: System::Void OutputFilesBox_Image_Click(System::Object^ sender, System::EventArgs^ e)
	{
		FolderBrowserDialog^ folderBrowserDialog = gcnew FolderBrowserDialog();

		// Set properties for the folder browser dialog
		folderBrowserDialog->Description = "Select a folder to output your SWF files to.";
		folderBrowserDialog->ShowNewFolderButton = false; // Optional: disable the "New Folder" button

		// Show the dialog and capture the result
		System::Windows::Forms::DialogResult result = folderBrowserDialog->ShowDialog();

		// Check if user selected a folder and proceed accordingly
		if (result == System::Windows::Forms::DialogResult::OK)
		{
			String^ selectedPath = folderBrowserDialog->SelectedPath;
			OutputFilesBox->Text = selectedPath;
		}
	}

	private: System::Void RunButton_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		int patchCount = 0;

		this->CurrentFile = "";

		//inventorylists.swf
		if (this->InventoryListsFilePathBox->Text != "Select 'InventoryLists.swf'")
		{
			this->CurrentFile = "inventorylists.swf";
			if (this->InventoryListsFilePathBox->Text != "")
			{
				this->LogMessage("Patching: " + toStdString(this->CurrentFile));
				standard::InventoryFormatter(toStdString(this->InventoryListsFilePathBox->Text), "InventoryListEntry");
				patchCount++;
			}
		}

		//craftingmenu.swf
		if (this->CraftingMenuFilePathBox->Text != "Select 'CraftingMenu.swf'")
		{
			this->CurrentFile = "craftingmenu.swf";
			if (this->CraftingMenuFilePathBox->Text != "")
			{
				this->LogMessage("Patching: " + toStdString(this->CurrentFile));
				standard::InventoryFormatter(toStdString(this->CraftingMenuFilePathBox->Text), "CraftingListEntry");
				patchCount++;
			}
		}

		//constructibleobjectmenu.swf
		if (this->ConstructibleObjectMenuFilePathBox->Text != "Select 'ConstructibleObjectMenu.swf'")
		{
			this->CurrentFile = "constructibleobjectmenu.swf";
			if (this->ConstructibleObjectMenuFilePathBox->Text != "")
			{
				this->LogMessage("Patching: " + toStdString(this->CurrentFile));
				standard::InventoryFormatter(toStdString(this->ConstructibleObjectMenuFilePathBox->Text), "CraftingListEntry");
				patchCount++;
			}
		}

		//LootMenu
		if (this->QuickLootFilePathBox->Text != "Select 'LootMenu.swf'")
		{
			this->CurrentFile = "lootmenu.swf";
			if (this->QuickLootFilePathBox->Text != "")
			{
				this->LogMessage("Patching: " + toStdString(this->CurrentFile));
				standard::QuickLootFormatter(toStdString(this->QuickLootFilePathBox->Text), "ListItemRenderer");
				patchCount++;
			}
		}

		if (patchCount > 0) {
			this->LogMessage("Finished patching...");
			return;
		}
		else
		{
			this->LogMessage("No patches applied...");
		}
	}

	private: System::Void CopyLog_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (!String::IsNullOrEmpty(log->Text))
		{
			Clipboard::SetText(this->log->Text);
			MessageBox::Show("Log copied to clipboard", "Completionist Automated UI Patcher", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}		
	}

	private: System::Void About_Click(System::Object^ sender, System::EventArgs^ e) {
		String^ aboutText =
			"Completionist Automated UI Patcher by Faen90\n"
			"Version: 1.0.5\n\n"
			"Description:\n"
			"An Automated SWF Patcher to easily add Compeltionist functionality to any supported SWF.\n\n"
			"Features:\n"
			"- Actionscript Extraction & Modification\n"
			"- Compiling of SWF files (ffdec)\n\n"
			"System Requirements:\n"
			"- Operating System: Windows 10 or later\n"
			"- Storage: 50 MB available space\n\n"
			"License:\n"
			"This software is licensed under the BSD-3-Clause License. See accompanying license.txt for details.\n\n"
			"Support:\n"
			"For support or inquiries, please contact me on discord @faen90.";

		MessageBox::Show(aboutText, "About...", MessageBoxButtons::OK, MessageBoxIcon::Information);
	}

	private: System::Void CreateNewModFolder_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
	{
		if (this->CreateNewModFolder->Checked)
		{
			FolderBrowserDialog^ folderBrowserDialog = gcnew FolderBrowserDialog();

			// Set properties for the folder browser dialog
			folderBrowserDialog->Description = "Select MO2 base directory.";
			folderBrowserDialog->ShowNewFolderButton = false; // Optional: disable the "New Folder" button

			// Show the dialog and capture the result
			System::Windows::Forms::DialogResult result = folderBrowserDialog->ShowDialog();

			// Check if user selected a folder and proceed accordingly
			if (result == System::Windows::Forms::DialogResult::OK)
			{
				String^ selectedPath = folderBrowserDialog->SelectedPath;
				String^ modOrganizerPath = System::IO::Path::Combine(selectedPath, "ModOrganizer.exe");

				// If the file does not exist, show a warning and reset the checkbox and other UI elements
				if (!System::IO::File::Exists(modOrganizerPath))
				{
					System::Windows::Forms::MessageBox::Show("The selected folder does not contain 'ModOrganizer.exe'. Please select a valid folder.", "MO2 Not Found", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Warning);
					ResetToDefaultState();
					return;
				}

				// If the file exists, update the OutputFilesBox and hide the OutputFilesBox_Image
				OutputFilesBox->Text = selectedPath;
				this->OutputFilesBox_Image->Visible = false;
			}
			else
			{
				// If the dialog was cancelled, reset the checkbox and show the OutputFilesBox_Image
				ResetToDefaultState();
				return;
			}
		}
		else
		{
			// If the checkbox is unchecked, reset to the default state
			ResetToDefaultState();
		}
	}

		   // Helper method to reset UI elements to their default state
	private: System::Void ResetToDefaultState()
	{
		this->CreateNewModFolder->Checked = false;
		this->OutputFilesBox->Text = "{Current Directory}/output";
		this->OutputFilesBox_Image->Visible = true;
	}
};
}
