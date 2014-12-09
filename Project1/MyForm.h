#include "rtpipv4address.h"
#include "../../CapstoneHeaders/pandaheader.h"
#ifndef WIN32
	#include <netinet/in.h>
	#include <arpa/inet.h>
#else
	#include <winsock2.h>
#endif // WIN32
#pragma once
namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm()
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		void send_select(bool * selected);
		void pollPackets();
		void set_address( UINT addr);
		void send_upd_packet();
		void stream();
		void stop_stream();

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
	private: System::Windows::Forms::Label^  label1;

	protected:


	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  importSettingsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exportSettingsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  startServerToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^  toolsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  getHelpToolStripMenuItem;
	private: System::Windows::Forms::PictureBox^  pictureBox1;

	private: System::Windows::Forms::Button^  stream_btn;

	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::CheckedListBox^  Devices;
	private: System::Windows::Forms::Timer^  timer2;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->importSettingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exportSettingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->startServerToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->getHelpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->stream_btn = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->Devices = (gcnew System::Windows::Forms::CheckedListBox());
			this->timer2 = (gcnew System::Windows::Forms::Timer(this->components));
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36));
			this->label1->Location = System::Drawing::Point(26, 53);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(286, 55);
			this->label1->TabIndex = 0;
			this->label1->Text = L"PandaClient";
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->fileToolStripMenuItem,
					this->helpToolStripMenuItem, this->toolsToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(642, 24);
			this->menuStrip1->TabIndex = 3;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->importSettingsToolStripMenuItem,
					this->exportSettingsToolStripMenuItem, this->exitToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// importSettingsToolStripMenuItem
			// 
			this->importSettingsToolStripMenuItem->Name = L"importSettingsToolStripMenuItem";
			this->importSettingsToolStripMenuItem->Size = System::Drawing::Size(155, 22);
			this->importSettingsToolStripMenuItem->Text = L"Import Settings";
			// 
			// exportSettingsToolStripMenuItem
			// 
			this->exportSettingsToolStripMenuItem->Name = L"exportSettingsToolStripMenuItem";
			this->exportSettingsToolStripMenuItem->Size = System::Drawing::Size(155, 22);
			this->exportSettingsToolStripMenuItem->Text = L"Export Settings";
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(155, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::exitToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->startServerToolStripMenuItem });
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->helpToolStripMenuItem->Text = L"Tools";
			// 
			// startServerToolStripMenuItem
			// 
			this->startServerToolStripMenuItem->Name = L"startServerToolStripMenuItem";
			this->startServerToolStripMenuItem->Size = System::Drawing::Size(132, 22);
			this->startServerToolStripMenuItem->Text = L"Find Server";
			// 
			// toolsToolStripMenuItem
			// 
			this->toolsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->getHelpToolStripMenuItem });
			this->toolsToolStripMenuItem->Name = L"toolsToolStripMenuItem";
			this->toolsToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->toolsToolStripMenuItem->Text = L"Help";
			// 
			// getHelpToolStripMenuItem
			// 
			this->getHelpToolStripMenuItem->Name = L"getHelpToolStripMenuItem";
			this->getHelpToolStripMenuItem->Size = System::Drawing::Size(120, 22);
			this->getHelpToolStripMenuItem->Text = L"Get Help";
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(421, 53);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(127, 97);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox1->TabIndex = 4;
			this->pictureBox1->TabStop = false;
			// 
			// stream_btn
			// 
			this->stream_btn->BackColor = System::Drawing::Color::Black;
			this->stream_btn->FlatAppearance->BorderColor = System::Drawing::Color::Lime;
			this->stream_btn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->stream_btn->Location = System::Drawing::Point(36, 395);
			this->stream_btn->Name = L"stream_btn";
			this->stream_btn->Size = System::Drawing::Size(150, 35);
			this->stream_btn->TabIndex = 6;
			this->stream_btn->Text = L"Start Streaming";
			this->stream_btn->UseVisualStyleBackColor = false;
			this->stream_btn->Click += gcnew System::EventHandler(this, &MyForm::stream_btn_Click);
			// 
			// button3
			// 
			this->button3->BackColor = System::Drawing::Color::Black;
			this->button3->FlatAppearance->BorderColor = System::Drawing::Color::Lime;
			this->button3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button3->Location = System::Drawing::Point(36, 449);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(150, 35);
			this->button3->TabIndex = 7;
			this->button3->Text = L"Stop Streaming";
			this->button3->UseVisualStyleBackColor = false;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// button4
			// 
			this->button4->BackColor = System::Drawing::Color::Black;
			this->button4->FlatAppearance->BorderColor = System::Drawing::Color::Lime;
			this->button4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button4->Location = System::Drawing::Point(192, 449);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(150, 35);
			this->button4->TabIndex = 8;
			this->button4->Text = L"Check For Devices";
			this->button4->UseVisualStyleBackColor = false;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14));
			this->label2->Location = System::Drawing::Point(372, 423);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(66, 24);
			this->label2->TabIndex = 9;
			this->label2->Text = L"My IP :";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(445, 426);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(126, 20);
			this->textBox1->TabIndex = 10;
			this->textBox1->Leave += gcnew System::EventHandler(this, &MyForm::textBox1_Enter);
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// Devices
			// 
			this->Devices->BackColor = System::Drawing::Color::Gray;
			this->Devices->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->Devices->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->Devices->ForeColor = System::Drawing::Color::Lime;
			this->Devices->FormattingEnabled = true;
			this->Devices->Location = System::Drawing::Point(47, 150);
			this->Devices->Name = L"Devices";
			this->Devices->Size = System::Drawing::Size(501, 210);
			this->Devices->TabIndex = 11;
			this->Devices->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::Devices_SelectedIndexChanged);
			// 
			// timer2
			// 
			this->timer2->Tick += gcnew System::EventHandler(this, &MyForm::timer2_Tick);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Black;
			this->ClientSize = System::Drawing::Size(642, 496);
			this->Controls->Add(this->Devices);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->stream_btn);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->menuStrip1);
			this->ForeColor = System::Drawing::Color::Lime;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"PandaClient";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
		printf("stopping stream.. \n");
		stop_stream();
		this->timer2->Stop();
		this->timer1->Start();
		//send_upd_packet();
	}
	private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		this->Close();
	}
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
		//pollPackets();
		send_upd_packet();
	}

	private: System::Void textBox1_Enter(System::Object^  sender, System::EventArgs^  e){
		IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(this->textBox1->Text);
		UINT destip;
		if (strlen(static_cast<char*>(ptrToNativeString.ToPointer())) > 10)
			inet_pton(AF_INET, static_cast<char*>(ptrToNativeString.ToPointer()), &destip);
		destip = ntohl(destip);
		set_address(destip);
		this->timer1->Interval = 20;
		this->timer1 -> Start();
	}

	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
		pollPackets();
	}
	private: System::Void Devices_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		bool checked[NUM_MAX_DEVICES];
		int i;
		for (i = 0; i < NUM_MAX_DEVICES; i++){
			checked[i] = this->Devices->GetItemChecked(i);
		}
		send_select( checked );
	}
	private: System::Void timer2_Tick(System::Object^  sender, System::EventArgs^  e) {
		stream();
	}
	private: System::Void stream_btn_Click(System::Object^  sender, System::EventArgs^  e) {
		printf("streaming audio.. \n");
		this->timer1->Stop();
		this->timer2->Interval = 100;
		this->timer2->Start();
	}
};
}

