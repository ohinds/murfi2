#include "FrUpdateTabsCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrMainController.h"
#include "FrBookmarkWidget.h"
#include "FrTabSettingsDocObj.h"
#include "FrMyLayeredImage.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"

#define BAD_TAB_ID -1


FrUpdateTabsCmd::FrUpdateTabsCmd()
: m_Action(FrUpdateTabsCmd::None), m_TabSettingsDocObj(0L), m_TabID(BAD_TAB_ID){

}

bool FrUpdateTabsCmd::Execute(){
    // Check params
    if(!this->GetMainController() || m_Action == FrUpdateTabsCmd::None) return false;

    bool result = false;
    if(m_Action == FrUpdateTabsCmd::SetCurrentTab){
        result = this->SetupTab();
    }
    else {
        result = this->UpdateView();
    }
    return result;
}

bool FrUpdateTabsCmd::SetupTab(){
    // check params
    if(m_TabID == BAD_TAB_ID) return false;

    // Update current tab in document!
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    std::vector<FrDocumentObj*> docTabs;
    doc->GetObjectsByType(docTabs, FrDocumentObj::TabSettings);
    
    std::vector<FrDocumentObj*>::iterator it, itEnd(docTabs.end());
    for(it = docTabs.begin(); it != itEnd; ++it){
        FrTabSettingsDocObj* tsDO = (FrTabSettingsDocObj*)(*it);
        tsDO->SetIsCurrent( (tsDO->GetID() == m_TabID) );
    }

    // Setup layers to images
    SetupImageLayers();

    return true;
}

bool FrUpdateTabsCmd::UpdateView(){
    // check params
    if(m_TabSettingsDocObj == 0L) return false;
    // Performe action in view
    FrBookmarkWidget* bmWidget = 
        this->GetMainController()->GetMainView()->GetBookmarkWidget();

    bool result = false;
    switch(m_Action){
        case FrUpdateTabsCmd::Add: ;
            result = bmWidget->AddBookmark(m_TabSettingsDocObj);
            break;
        case FrUpdateTabsCmd::Remove: 
            result = bmWidget->RemoveBookmark(m_TabSettingsDocObj);
            break;
        case FrUpdateTabsCmd::Update:
            // TODO: Implement !!!
            //result = bmWidget->UpdateTab(m_TabSettingsDocObj);
            break;
    }
    return result;
}


void FrUpdateTabsCmd::SetupImageLayers(){
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = doc->GetCurrentTabSettings();
    
    FrMyLayeredImage* img = mv->GetSliceView()->GetImage();
    LayerCollection& layers = tabSets->GetSliceViewSettings()->OtherLayers;
    
    

    //FrSliceView* sv = ;
    //FrMosaicView* 
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrUpdateTabsCmd::CanUndo(){
    return false;
}

bool FrUpdateTabsCmd::Undo(){
    return false;
}

bool FrUpdateTabsCmd::Redo(){
    return false;
}