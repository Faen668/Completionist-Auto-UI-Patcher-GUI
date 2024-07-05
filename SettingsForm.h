#pragma once

namespace CompletionistAutoUIPatcherGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Summary for SettingsForm
	/// </summary>
	public ref class SettingsForm : public System::Windows::Forms::Form
	{
	public:
		SettingsForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SettingsForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::CheckBox^ CraftingMenuFilePathBox_Enabled;
	public: System::Windows::Forms::CheckBox^ ConstructibleObjectMenuFilePathBox_Enabled;
	public: System::Windows::Forms::CheckBox^ QuickLootFilePathBox_Enabled;
	protected:

	public:



		System::Windows::Forms::TextBox^ CraftingMenuFilePathBox;
	public: System::Windows::Forms::TextBox^ ConstructibleObjectMenuFilePathBox;
	public: System::Windows::Forms::TextBox^ QuickLootFilePathBox;



		System::Windows::Forms::Label^ label1;

	
		System::Windows::Forms::CheckBox^ InventoryListsFilePathBox_Enabled;
		System::Windows::Forms::TextBox^ InventoryListsFilePathBox;

		System::Windows::Forms::TextBox^ OutputFilesBox;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->InventoryListsFilePathBox_Enabled = (gcnew System::Windows::Forms::CheckBox());
			this->CraftingMenuFilePathBox_Enabled = (gcnew System::Windows::Forms::CheckBox());
			this->ConstructibleObjectMenuFilePathBox_Enabled = (gcnew System::Windows::Forms::CheckBox());
			this->QuickLootFilePathBox_Enabled = (gcnew System::Windows::Forms::CheckBox());
			this->InventoryListsFilePathBox = (gcnew System::Windows::Forms::TextBox());
			this->CraftingMenuFilePathBox = (gcnew System::Windows::Forms::TextBox());
			this->ConstructibleObjectMenuFilePathBox = (gcnew System::Windows::Forms::TextBox());
			this->QuickLootFilePathBox = (gcnew System::Windows::Forms::TextBox());
			this->OutputFilesBox = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// InventoryListsFilePathBox_Enabled
			// 
			this->InventoryListsFilePathBox_Enabled->AutoSize = true;
			this->InventoryListsFilePathBox_Enabled->Location = System::Drawing::Point(12, 12);
			this->InventoryListsFilePathBox_Enabled->Name = L"InventoryListsFilePathBox_Enabled";
			this->InventoryListsFilePathBox_Enabled->Size = System::Drawing::Size(276, 29);
			this->InventoryListsFilePathBox_Enabled->TabIndex = 0;
			this->InventoryListsFilePathBox_Enabled->Text = L"Patch InventoryLists.swf";
			this->InventoryListsFilePathBox_Enabled->UseVisualStyleBackColor = true;
			// 
			// CraftingMenuFilePathBox_Enabled
			// 
			this->CraftingMenuFilePathBox_Enabled->AutoSize = true;
			this->CraftingMenuFilePathBox_Enabled->Location = System::Drawing::Point(12, 102);
			this->CraftingMenuFilePathBox_Enabled->Name = L"CraftingMenuFilePathBox_Enabled";
			this->CraftingMenuFilePathBox_Enabled->Size = System::Drawing::Size(272, 29);
			this->CraftingMenuFilePathBox_Enabled->TabIndex = 1;
			this->CraftingMenuFilePathBox_Enabled->Text = L"Patch CraftingMenu.swf";
			this->CraftingMenuFilePathBox_Enabled->UseVisualStyleBackColor = true;
			// 
			// ConstructibleObjectMenuFilePathBox_Enabled
			// 
			this->ConstructibleObjectMenuFilePathBox_Enabled->AutoSize = true;
			this->ConstructibleObjectMenuFilePathBox_Enabled->Location = System::Drawing::Point(12, 208);
			this->ConstructibleObjectMenuFilePathBox_Enabled->Name = L"ConstructibleObjectMenuFilePathBox_Enabled";
			this->ConstructibleObjectMenuFilePathBox_Enabled->Size = System::Drawing::Size(385, 29);
			this->ConstructibleObjectMenuFilePathBox_Enabled->TabIndex = 2;
			this->ConstructibleObjectMenuFilePathBox_Enabled->Text = L"Patch ConstructibleObjectMenu.swf";
			this->ConstructibleObjectMenuFilePathBox_Enabled->UseVisualStyleBackColor = true;
			// 
			// QuickLootFilePathBox_Enabled
			// 
			this->QuickLootFilePathBox_Enabled->AutoSize = true;
			this->QuickLootFilePathBox_Enabled->Location = System::Drawing::Point(12, 303);
			this->QuickLootFilePathBox_Enabled->Name = L"QuickLootFilePathBox_Enabled";
			this->QuickLootFilePathBox_Enabled->Size = System::Drawing::Size(239, 29);
			this->QuickLootFilePathBox_Enabled->TabIndex = 3;
			this->QuickLootFilePathBox_Enabled->Text = L"Patch LootMenu.swf";
			this->QuickLootFilePathBox_Enabled->UseVisualStyleBackColor = true;
			// 
			// InventoryListsFilePathBox
			// 
			this->InventoryListsFilePathBox->Location = System::Drawing::Point(12, 47);
			this->InventoryListsFilePathBox->Name = L"InventoryListsFilePathBox";
			this->InventoryListsFilePathBox->Size = System::Drawing::Size(531, 31);
			this->InventoryListsFilePathBox->TabIndex = 4;
			this->InventoryListsFilePathBox->Click += gcnew System::EventHandler(this, &SettingsForm::InventoryListsFilePathBox_Click);
			// 
			// CraftingMenuFilePathBox
			// 
			this->CraftingMenuFilePathBox->Location = System::Drawing::Point(12, 139);
			this->CraftingMenuFilePathBox->Name = L"CraftingMenuFilePathBox";
			this->CraftingMenuFilePathBox->Size = System::Drawing::Size(531, 31);
			this->CraftingMenuFilePathBox->TabIndex = 5;
			this->CraftingMenuFilePathBox->Click += gcnew System::EventHandler(this, &SettingsForm::CraftingMenuFilePathBox_Click);
			// 
			// ConstructibleObjectMenuFilePathBox
			// 
			this->ConstructibleObjectMenuFilePathBox->Location = System::Drawing::Point(12, 243);
			this->ConstructibleObjectMenuFilePathBox->Name = L"ConstructibleObjectMenuFilePathBox";
			this->ConstructibleObjectMenuFilePathBox->Size = System::Drawing::Size(531, 31);
			this->ConstructibleObjectMenuFilePathBox->TabIndex = 6;
			this->ConstructibleObjectMenuFilePathBox->Click += gcnew System::EventHandler(this, &SettingsForm::ConstructibleObjectMenuFilePathBox_Click);
			// 
			// QuickLootFilePathBox
			// 
			this->QuickLootFilePathBox->Location = System::Drawing::Point(12, 338);
			this->QuickLootFilePathBox->Name = L"QuickLootFilePathBox";
			this->QuickLootFilePathBox->Size = System::Drawing::Size(531, 31);
			this->QuickLootFilePathBox->TabIndex = 7;
			this->QuickLootFilePathBox->Click += gcnew System::EventHandler(this, &SettingsForm::QuickLootFilePathBox_Click);
			// 
			// OutputFilesBox
			// 
			this->OutputFilesBox->Location = System::Drawing::Point(12, 420);
			this->OutputFilesBox->Name = L"OutputFilesBox";
			this->OutputFilesBox->Size = System::Drawing::Size(531, 31);
			this->OutputFilesBox->TabIndex = 8;
			this->OutputFilesBox->Text = L"/output";
			this->OutputFilesBox->Click += gcnew System::EventHandler(this, &SettingsForm::OutputFilesBox_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 392);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(429, 25);
			this->label1->TabIndex = 9;
			this->label1->Text = L"Place Compiled SWF Files In This Directory";
			// 
			// SettingsForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(12, 25);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(663, 488);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->OutputFilesBox);
			this->Controls->Add(this->QuickLootFilePathBox);
			this->Controls->Add(this->ConstructibleObjectMenuFilePathBox);
			this->Controls->Add(this->CraftingMenuFilePathBox);
			this->Controls->Add(this->InventoryListsFilePathBox);
			this->Controls->Add(this->QuickLootFilePathBox_Enabled);
			this->Controls->Add(this->ConstructibleObjectMenuFilePathBox_Enabled);
			this->Controls->Add(this->CraftingMenuFilePathBox_Enabled);
			this->Controls->Add(this->InventoryListsFilePathBox_Enabled);
			this->Name = L"SettingsForm";
			this->Text = L"Completionist UI Patcher Settings...";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void InventoryListsFilePathBox_Click(System::Object^ sender, System::EventArgs^ e) 
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
	private: System::Void CraftingMenuFilePathBox_Click(System::Object^ sender, System::EventArgs^ e) 
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

	private: System::Void ConstructibleObjectMenuFilePathBox_Click(System::Object^ sender, System::EventArgs^ e) 
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

	private: System::Void QuickLootFilePathBox_Click(System::Object^ sender, System::EventArgs^ e) 
	{
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

	private: System::Void OutputFilesBox_Click(System::Object^ sender, System::EventArgs^ e) 
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
			// Do something with the selected folder path, e.g., update a text box
			OutputFilesBox->Text = selectedPath;
		}
	}
};
}
