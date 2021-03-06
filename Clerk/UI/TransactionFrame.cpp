#include "TransactionFrame.h"

TransactionFrame::TransactionFrame(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(* wxWHITE));

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxFloatingPointValidator<float> amountValidator(2, &fromValue, wxNUM_VAL_DEFAULT);
	amountValidator.SetRange(0.0f, 999999999.0f);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *lineSizer = new wxBoxSizer(wxHORIZONTAL);

	fromLabel = new wxStaticText(mainPanel, wxID_ANY, "From:", wxDefaultPosition, wxSize(40, -1), 0);
	lineSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromList = new wxBitmapComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	lineSizer->Add(fromList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromAmountField = new wxTextCtrl(mainPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT, amountValidator);
	lineSizer->Add(fromAmountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromAmountLabel = new wxStaticText(mainPanel, wxID_ANY, "RUB", wxDefaultPosition, wxSize(40, -1), 0);
	lineSizer->Add(fromAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(lineSizer, 0, wxALL | wxEXPAND, 5);

	lineSizer = new wxBoxSizer(wxHORIZONTAL);

	toLabel = new wxStaticText(mainPanel, wxID_ANY, "To:", wxDefaultPosition, wxSize(40, -1), 0);
	lineSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toList = new wxBitmapComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	lineSizer->Add(toList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toAmountField = new wxTextCtrl(mainPanel, wxID_ANY, "0.00", wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT, amountValidator);
	lineSizer->Add(toAmountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toAmountLabel = new wxStaticText(mainPanel, wxID_ANY, "RUB", wxDefaultPosition, wxSize(40, -1), 0);
	lineSizer->Add(toAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(lineSizer, 0, wxALL | wxEXPAND, 5);
	
	lineSizer = new wxBoxSizer(wxHORIZONTAL);

	dateLabel = new wxStaticText(mainPanel, wxID_ANY, "Date:", wxDefaultPosition, wxSize(40, -1), 0);
	lineSizer->Add(dateLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	datePicker = new wxDatePickerCtrl(mainPanel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);
	lineSizer->Add(datePicker, 0, wxALL, 5);

	panelSizer->Add(lineSizer, 0, wxALL | wxEXPAND, 5);

	lineSizer = new wxBoxSizer(wxHORIZONTAL);

	tagsLabel = new wxStaticText(mainPanel, wxID_ANY, "Tags:", wxDefaultPosition, wxSize(40, -1), 0);
	lineSizer->Add(tagsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	tagsField = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	lineSizer->Add(tagsField, 1, wxALL | wxEXPAND, 5);

	panelSizer->Add(lineSizer, 0, wxALL | wxEXPAND, 5);

	lineSizer = new wxBoxSizer(wxHORIZONTAL);

	noteLabel = new wxStaticText(mainPanel, wxID_ANY, "Note:", wxDefaultPosition, wxSize(40, -1), 0);
	lineSizer->Add(noteLabel, 0, wxALL, 5);

	noteField = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	lineSizer->Add(noteField, 1, wxALL | wxEXPAND, 5);

	panelSizer->Add(lineSizer, 1, wxALL | wxEXPAND, 5);
	
	lineSizer = new wxBoxSizer(wxHORIZONTAL);

	okButton = new wxButton(mainPanel, wxID_ANY, "OK", wxDefaultPosition, wxDefaultSize, 0);
	lineSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

	cancelButton = new wxButton(mainPanel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize, 0);
	lineSizer->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

	panelSizer->Add(lineSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	mainPanel->SetSizer(panelSizer);
	mainPanel->Layout();

	panelSizer->Fit(mainPanel);

	mainSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 0);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	tagsPopup = new TagsPopup(this);	
	tagsPopup->OnSelectTag = std::bind(&TransactionFrame::OnSelectTag, this);

	okButton->Bind(wxEVT_BUTTON, &TransactionFrame::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &TransactionFrame::OnCancel, this);

	fromList->Bind(wxEVT_COMBOBOX, &TransactionFrame::OnFromAccountSelect, this);
	toList->Bind(wxEVT_COMBOBOX, &TransactionFrame::OnToAccountSelect, this);
	fromAmountField->Bind(wxEVT_KILL_FOCUS, &TransactionFrame::OnFromAmountKillFocus, this);
	tagsField->Bind(wxEVT_KEY_UP, &TransactionFrame::OnTextChanged, this);
	tagsField->Bind(wxEVT_KILL_FOCUS, &TransactionFrame::OnTagsKillFocus, this);

	fromValue = 0;
	toValue = 0;

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Receipt))
	{
		accounts.push_back(account);
	}

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Deposit))
	{
		accounts.push_back(account);
	}

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Expens))
	{
		accounts.push_back(account);
	}

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Debt))
	{
		accounts.push_back(account);
	}

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Credit))
	{
		accounts.push_back(account);
	}

	accountsImageList = new wxImageList(16, 16, false);

	for (int i = 0; i <= 50; i++) {
		wxString path = wxString::Format("Resources\\Accounts Icons\\%d.png", i);
		wxImage image(path);

		if (image.IsOk())
		{
			wxBitmap *bitmap = new wxBitmap(image);
			accountsImageList->Add(*bitmap);

			delete bitmap;
		}
	}

	UpdateFromList();
	SelectFromAccount(0);

	UpdateToList(fromAccount);
	SelectToAccount(0);
}

TransactionFrame::~TransactionFrame() {
	delete fromLabel;
	delete toLabel;
	delete fromList;
	delete toList;
	delete fromAmountField;
	delete fromAmountLabel;
	delete okButton;
	delete cancelButton;
	delete tagsLabel;
	delete tagsField;
	delete noteLabel;
	delete noteField;
	delete dateLabel;
	delete datePicker;
	delete accountsImageList;
	delete tagsPopup;	
}

void TransactionFrame::UpdateFromList() {
	for each (auto account in accounts) {
		if (account->type == AccountTypes::Receipt || account->type == AccountTypes::Deposit) {
			int icon = 0;

			if (account->iconId < accountsImageList->GetImageCount()) {
				icon = account->iconId;
			}

			fromList->Append(*account->name, accountsImageList->GetBitmap(icon));

			fromAccounts.push_back(account);
		}
	}
}

void TransactionFrame::UpdateToList(std::shared_ptr<Account> account) {
	toList->Clear();
	toAccounts.clear();

	for each (auto toAccount in accounts)
	{
		if (account->id == toAccount->id) {
			continue;
		}

		if (account->type == AccountTypes::Receipt) {
			if (toAccount->type == AccountTypes::Deposit) {
				int icon = 0;

				if (toAccount->iconId < accountsImageList->GetImageCount()) {
					icon = toAccount->iconId;
				}

				toList->Append(*toAccount->name, accountsImageList->GetBitmap(icon));

				toAccounts.push_back(toAccount);
			}
		}
		else if (account->type == AccountTypes::Deposit) {
			if (toAccount->type == AccountTypes::Deposit || toAccount->type == AccountTypes::Expens || toAccount->type == AccountTypes::Debt || toAccount->type == AccountTypes::Credit) {
				int icon = 0;

				if (toAccount->iconId < accountsImageList->GetImageCount()) {
					icon = toAccount->iconId;
				}

				toList->Append(*toAccount->name, accountsImageList->GetBitmap(icon));

				toAccounts.push_back(toAccount);
			}
		}
	}
}

void TransactionFrame::SelectFromAccount(int index) {
	auto account = fromAccounts[index];

	fromList->Select(index);
	fromAmountLabel->SetLabel(*account->currency->shortName);
	fromAccount = account;
}

void TransactionFrame::SelectToAccount(int id) {
	auto account = toAccounts[id];

	toList->Select(id);
	toAmountLabel->SetLabel(*account->currency->shortName);

	toAccount = account;
}

void TransactionFrame::SelectToAccount(std::shared_ptr<Account> account) {
	for (unsigned int i = 0; i < toAccounts.size(); i++) {
		if (toAccounts[i]->id == account->id) {
			SelectToAccount(i);
			return;
		}
	}

	SelectToAccount(0);
}

void TransactionFrame::OnFromAccountSelect(wxCommandEvent &event) {
	SelectFromAccount(fromList->GetSelection());

	UpdateToList(fromAccount);
	SelectToAccount(toAccount);
}

void TransactionFrame::OnToAccountSelect(wxCommandEvent &event) {
	SelectToAccount(toList->GetSelection());
}

void TransactionFrame::SetTransaction(std::shared_ptr<Transaction> transaction) {
	this->transaction = transaction;

	fromAmountField->SetValue(wxString::Format("%.2f", transaction->fromAmount));
	toAmountField->SetValue(wxString::Format("%.2f", transaction->toAmount));
	tagsField->SetValue(*transaction->tags);
	noteField->SetValue(*transaction->note);
	datePicker->SetValue(*transaction->paidAt);

	for (unsigned int i = 0; i < fromAccounts.size(); i++) {
		if (transaction->fromAccountId == fromAccounts[i]->id) {
			SelectFromAccount(i);
			UpdateToList(fromAccounts[i]);

			break;
		}
	}

	if (transaction->toAccountId != -1) {
		for (unsigned int i = 0; i < toAccounts.size(); i++) {
			if (transaction->toAccountId == toAccounts[i]->id) {
				SelectToAccount(i);
				break;
			}
		}
	}
	else {
		SelectToAccount(0);
	}	

	fromAmountField->SetFocus();
	fromAmountField->SelectAll();
}

void TransactionFrame::SetSplitTransaction(std::shared_ptr<Transaction> transaction) {
	auto copy = make_shared<Transaction>();

	copy->fromAccountId = transaction->fromAccountId;
	copy->toAccountId = transaction->toAccountId;
	copy->fromAmount = transaction->fromAmount;
	copy->toAmount = 0.0;
	copy->paidAt = make_shared<wxDateTime>(wxDateTime::Now());

	this->transaction = copy;
	this->splitTransaction = transaction;

	fromAmountField->SetValue(wxString::Format("%.2f", this->transaction->fromAmount));
	toAmountField->SetValue(wxString::Format("%.2f", 0.0));
	tagsField->SetValue("");
	noteField->SetValue("");
	datePicker->SetValue(*this->transaction->paidAt);

	for (unsigned int i = 0; i < fromAccounts.size(); i++) {
		if (this->transaction->fromAccountId == fromAccounts[i]->id) {
			SelectFromAccount(i);
			UpdateToList(fromAccounts[i]);

			break;
		}
	}

	for (unsigned int i = 0; i < toAccounts.size(); i++) {
		if (this->transaction->toAccountId == toAccounts[i]->id) {
			SelectToAccount(i);
			break;
		}
	}

	fromAmountField->SetFocus();
	fromAmountField->SelectAll();
}

void TransactionFrame::OnOK(wxCommandEvent &event) {
	double val;

	wxString value = fromAmountField->GetValue();
	value.Replace(" ", "");
	value.Replace(",", ".");
	value.ToDouble(&val);
	  
	fromValue = val;
	
	value = toAmountField->GetValue();
	value.Replace(" ", "");
	value.Replace(",", ".");
	value.ToDouble(&val);

	toValue = val;

	transaction->fromAccountId = fromAccounts[fromList->GetSelection()]->id;
	transaction->toAccountId = toAccounts[toList->GetSelection()]->id;
	transaction->fromAmount = fromValue;
	transaction->toAmount = toValue;
	transaction->note = make_shared<wxString>(noteField->GetValue());
	transaction->tags = make_shared<wxString>(tagsField->GetValue());
	transaction->paidAt = make_shared<wxDateTime>(datePicker->GetValue());

	transaction->Save();

	if (splitTransaction) {
		splitTransaction->fromAmount = splitTransaction->fromAmount - transaction->fromAmount;
		splitTransaction->toAmount = splitTransaction->toAmount - transaction->toAmount;

		splitTransaction->Save();
	}

	Close();

	if (OnClose) {
		OnClose();
	}
}

void TransactionFrame::OnCancel(wxCommandEvent &event) {
	Close();
}

void TransactionFrame::OnFromAmountKillFocus(wxFocusEvent &event) {
	event.Skip();
	
	double val;

	toAmountField->GetValue().ToDouble(&val);

	int fromCurrencyId = fromAccounts[fromList->GetSelection()]->currency->id;
	int toCurrencyId = toAccounts[toList->GetSelection()]->currency->id;

	if (val == 0 && fromCurrencyId == toCurrencyId) {
		toAmountField->SetValue(fromAmountField->GetValue());
	}
}

void TransactionFrame::OnTextChanged(wxKeyEvent &event) {
	if (event.GetKeyCode() == WXK_ESCAPE) {
		tagsPopup->Hide();
	} else if (event.GetKeyCode() == WXK_UP) {
		tagsPopup->SelectPrev();
		event.StopPropagation();
	} else if (event.GetKeyCode() == WXK_DOWN) {
		tagsPopup->SelectNext();
	} else if (event.GetKeyCode() == WXK_RETURN) {		
		AddTag();
		tagsPopup->Hide();
	} else {
		wxStringTokenizer tokenizer(tagsField->GetValue(), ",");
		vector<wxString> tokens;

		while (tokenizer.HasMoreTokens()) {
			wxString token = tokenizer.GetNextToken().Trim(true).Trim(false);
			tokens.push_back(token);
		}

		if (!tokens.empty()) {
			auto tags = DataHelper::GetInstance().GetTagsBySearch(tokens.back());

			if (!tokens.empty()) {
				tagsPopup->Update(tags);

				wxPoint pos = tagsField->GetScreenPosition();
				wxSize size = tagsField->GetSize();
				
				tagsPopup->Position(wxPoint(pos.x - 200, pos.y - 200 + size.GetHeight()), wxSize(200, 200));
				tagsPopup->Show();
			}
			else {
				tagsPopup->Hide();
			}
		}
	}

	event.Skip();
}

void TransactionFrame::OnTagsKillFocus(wxFocusEvent& event) {
	tagsPopup->Hide();
	event.Skip();
}

void TransactionFrame::OnSelectTag() {
	AddTag();
	tagsPopup->Hide();	
}

void TransactionFrame::AddTag() {
	wxString tag = tagsPopup->GetSelectedTag();
	wxString result = "";

	wxStringTokenizer tokenizer(tagsField->GetValue(), ",");
	vector<wxString> tokens;

	while (tokenizer.HasMoreTokens()) {
		wxString token = tokenizer.GetNextToken().Trim(true).Trim(false);
		tokens.push_back(token);
	}

	for (unsigned int i = 0; i < tokens.size() - 1; i++) {
		result.Append(tokens[i]);
		result.Append(", ");
	}

	result.Append(tag);

	tagsField->SetValue(result);
	tagsField->SetInsertionPointEnd();
}