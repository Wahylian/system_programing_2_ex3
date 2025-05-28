#include "gui.hpp"
#include <wx/textdlg.h>
#include <vector>
#include <string>

class GamePanel : public wxPanel {
public:
    GamePanel(wxWindow* parent, const std::vector<std::string>& playerNames)
        : wxPanel(parent), game(playerNames)
    {
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

        playerList = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr);
        mainSizer->Add(new wxStaticText(this, wxID_ANY, "Players:"), 0, wxALL, 5);
        mainSizer->Add(playerList, 0, wxALL | wxEXPAND, 5);

        infoText = new wxStaticText(this, wxID_ANY, "");
        mainSizer->Add(infoText, 0, wxALL, 5);

        actionChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr);
        mainSizer->Add(new wxStaticText(this, wxID_ANY, "Action:"), 0, wxALL, 5);
        mainSizer->Add(actionChoice, 0, wxALL | wxEXPAND, 5);

        targetChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr);
        mainSizer->Add(new wxStaticText(this, wxID_ANY, "Target (if needed):"), 0, wxALL, 5);
        mainSizer->Add(targetChoice, 0, wxALL | wxEXPAND, 5);

        wxButton* doActionBtn = new wxButton(this, wxID_ANY, "Do Action");
        mainSizer->Add(doActionBtn, 0, wxALL, 5);

        SetSizer(mainSizer);

        doActionBtn->Bind(wxEVT_BUTTON, &GamePanel::OnDoAction, this);

        UpdateUI();
    }

private:
    Game game;
    wxListBox* playerList;
    wxStaticText* infoText;
    wxChoice* actionChoice;
    wxChoice* targetChoice;

    void UpdateUI() {
        playerList->Clear();
        auto names = game.players();
        for (const auto& name : names)
            playerList->Append(name);

        auto info = game.info();
        wxString infoStr;
        infoStr << "Name: " << info[0] << "\nRole: " << info[1]
                << "\nCoins: " << info[2] << "\nActions left: " << info[3];
        infoText->SetLabel(infoStr);

        actionChoice->Clear();
        auto actions = game.validActions();
        for (const auto& act : actions)
            actionChoice->Append(act);

        targetChoice->Clear();
        for (size_t i = 0; i < names.size(); ++i)
            targetChoice->Append(names[i]);
    }

    void OnDoAction(wxCommandEvent&) {
        // get the action selected by the user
        wxString wxAction = actionChoice->GetStringSelection();
        // convert it to string
        string action = wxAction.ToStdString();

        // if the action is empty the user pressed the button without selecting an action
        if(action == "") {
            wxMessageBox("Please select an action");
            return;
        }
        
        // check that the action is valid for the current player
        if(!game.isValidAction(action)){
            // since in the gui the actions are predefined, 
            // this block will be entered only if the action is not coup, when the player 
            // has at least 1 action left and at least 10 coins
            wxMessageBox(this->game.turn() + " you have at least 10 coins, you must perform a Coup");
            return;
        }
            
        // get the index of the target player selected by the current player 
        int targetIndex = targetChoice->GetSelection();
        Player *target = nullptr;

        // check if the action requires a target player
        if(game.isTargetRequired(action)){
            // check if the target index provided is not found
            if(targetIndex == wxNOT_FOUND) {
                wxMessageBox("Please select a target player.");
                return;
            }
            // get the target player by their index
            target = game.getPlayerByIndex(targetIndex);
        }

        // attempt to perform the action
        try{
            // since in reality the coup is handled by the game itself,
            // the best way to handle an undo of the coup is to "undo" it before it takes place
            bool undoCoup = false;
            // make sure that the coup is valid and can be preformed by the player
            if(action == "coup"){
                if(game.isValidCoup(target)){
                    // check if there are any generals in the game (except for the current player)
                    vector<Player*> generals = game.allOfRole("General");
                    for(const Player *general : generals){
                        // to undo a coup the general must have at least 5 coins
                        // and so check if the general has at least 5 coins
                        if(general->coins() < 5) {
                            continue; // skip this general if they don't have enough coins
                        }
                        
                        // ask the player if they want to undo the coup
                        wxMessageDialog dlg(this,
                        wxString::Format("%s, would you want to undo the coup on %s?",
                        general->getName(), target->getName()), "Undo Coup?", wxYES_NO | wxICON_QUESTION);

                        if (dlg.ShowModal() == wxID_YES) {
                            // undo the coup
                            game.undoAction(*general, "undoCoup");
                            undoCoup = true;
                            break;
                        }
                    }
                }
            }

            // play the action
            int res = game.playAction(action, target, undoCoup);

            // if the result is -1, it means the turn is over
            if(res == -1 || action == "coup"){
                // if the turn ended, check if the game is over
                try{
                    wxMessageBox("The winner is: " + game.winner());
                    CloseParent(); // close the game window
                }
                catch (const game_not_over_exception&) {
                    // if the game is not over, continue to the next turn
                    wxMessageBox("The turn has ended, it's now " + game.turn() + "'s turn.");
                    UpdateUI(); // update the UI
                }
                return; // exit the function
            }

            // if the action was "spyOn", res will hold the number of coins the target player has
            if(action == "spyOn"){
                wxMessageBox(wxString::Format("%s has %d coins", target->getName(), res));
            }

            // check if the action was bribe
            if(action == "bribe"){
                // check if there are any judges in the game (except for the current player)
                vector<Player*> judges = game.allOfRole("Judge");
                for(const Player *judge : judges){
                    // ask the player if they want to undo the bribe
                    wxMessageDialog dlg(this,
                    wxString::Format("%s, would you want to undo %s's bribe?",
                    judge->getName(), game.turn()), "Undo Bribe?", wxYES_NO | wxICON_QUESTION);

                    if (dlg.ShowModal() == wxID_YES) {
                        // undo the bribe
                        game.undoAction(*judge, "undoBribe");
                        wxMessageBox(wxString::Format("%s's bribe was undone", game.turn()));
                        break;
                    }
                }
            }

            // check if the action was tax
            if(action == "tax"){
                // check if there are any governors in the game (except for the current player)
                vector<Player*> governors = game.allOfRole("Governor");
                for(const Player *governor : governors){
                    // ask the player if they want to undo the tax
                    wxMessageDialog dlg(this,
                    wxString::Format("%s, would you want to undo %s's tax?",
                    governor->getName(), game.turn()), "Undo Tax?", wxYES_NO | wxICON_QUESTION);

                    if (dlg.ShowModal() == wxID_YES) {
                        // undo the tax
                        game.undoAction(*governor, "undoTax");
                        wxMessageBox(wxString::Format("%s's tax action was undone", game.turn()));
                        break;
                    }
                }
            }
        }
        catch (const std::runtime_error &e){
            wxMessageBox(e.what());
        }
        catch (const std::invalid_argument &e){
            wxMessageBox(e.what());
        }
        catch (const std::exception &e){
            wxMessageBox("An unxpected error occurred: " + wxString(e.what()));
        }

        // update the UI after the action is performed
        UpdateUI(); 
    }

    void CloseParent() {
        wxWindow* top = wxGetTopLevelParent(this);
        if (top) top->Close();
    }
};

bool MyApp::OnInit() {
    // Ask for number of players
    long numPlayers = 2;
    wxTextEntryDialog numDialog(NULL, "Enter number of players (2-6):", "Number of Players", "2");
    numDialog.SetTextValidator(wxFILTER_DIGITS);
    if (numDialog.ShowModal() != wxID_OK)
        return false;
    numDialog.GetValue().ToLong(&numPlayers);
    if (numPlayers < 2) numPlayers = 2;
    if (numPlayers > 6) numPlayers = 6;

    // Ask for each player's name
    std::vector<std::string> playerNames;
    for (long i = 0; i < numPlayers; ++i) {
        wxString prompt = wxString::Format("Enter name for player %ld:", i + 1);
        wxTextEntryDialog nameDialog(NULL, prompt, "Player Name", wxString::Format("Player%ld", i + 1));
        if (nameDialog.ShowModal() != wxID_OK)
            return false;
        playerNames.push_back(nameDialog.GetValue().ToStdString());
    }

    MyFrame* frame = new MyFrame(playerNames);
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const std::vector<std::string>& playerNames)
    : wxFrame(NULL, wxID_ANY, "Coup Game GUI") {
    SetMinSize(wxSize(350, 450));
    SetMaxSize(wxSize(350, 450));
    new GamePanel(this, playerNames);
}

wxIMPLEMENT_APP(MyApp);