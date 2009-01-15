#ifndef FR_GRAPH_CMD
#define FR_GRAPH_CMD

class FrGraphDocObj;

#include "FrBaseCmd.h"
#include "FrMacro.h"

// This command allows user to add, remove 
// and modify settings of the layer
class FrGraphCmd : public FrBaseCmd {
public:
    typedef enum _Action { 
        Undefined, Add, Delete 
    } Actions;

public:
    // Constructor/destructor
    FrGraphCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Properties
    FrSetPropMacro(Actions, Action);    
    FrSetPropMacro(FrGraphDocObj*, DocObj);
    void SetID(int id);

private:
    // Helper methods here
    bool AddGraph();
    bool DeleteGraph();

    int GetActiveLayerID();
    bool IsRoiLayer(int id);

private:
    bool m_isID;
    int m_ID;
};

#endif