/***************************************************************************
 *   Copyright (c) 2004 Jürgen Riegel <juergen.riegel@web.de>              *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#ifndef GUI_TREE_H
#define GUI_TREE_H

#include <unordered_map>
#include <memory>
#include <QTreeWidget>
#include <QTime>
#include <QStyledItemDelegate>

#include <Base/Parameter.h>
#include <Base/Persistence.h>
#include <App/Document.h>
#include <App/Application.h>

#include <Gui/DockWindow.h>
#include <Gui/Selection.h>

class QLineEdit;

namespace App {
class SubObjectT;
}

namespace Gui {

class SelUpMenu;
class TreeParams;
class ViewProviderDocumentObject;
class DocumentObjectItem;
class DocumentObjectData;
typedef std::shared_ptr<DocumentObjectData> DocumentObjectDataPtr;
class TreeWidgetItemDelegate;

class DocumentItem;

/// highlight modes for the tree items
enum class HighlightMode {
    Underlined,
    Italic,
    Overlined,
    Bold,
    Blue,
    LightBlue,
    UserDefined
};

/// highlight modes for the tree items
enum class TreeItemMode {
    ExpandItem,
    ExpandPath,
    CollapseItem,
    ToggleItem
};


GuiExport int treeViewIconSize();
GuiExport bool isTreeViewDragging();
GuiExport int isTreeViewDropping();


/** Tree view that allows drag & drop of document objects.
 * @author Werner Mayer
 */
class TreeWidget : public QTreeWidget, public SelectionObserver
{
    Q_OBJECT

    Q_PROPERTY(int iconHeight READ iconHeight WRITE setIconHeight DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(int itemSpacing READ itemSpacing WRITE setItemSpacing DESIGNABLE true SCRIPTABLE true)

public:
    TreeWidget(const char *name, QWidget* parent=0);
    ~TreeWidget();

    static void setupResizableColumn(TreeWidget *tree=0);
    static void scrollItemToTop();
    static void selectAllInstances(const ViewProviderDocumentObject &vpd);
    static void selectLinkedObject(App::DocumentObject *linked);
    static void selectAllLinks(App::DocumentObject *obj);
    static void populateSelUpMenu(QMenu *menu, const App::SubObjectT *objT=nullptr);
    static void execSelUpMenu(SelUpMenu *menu, const QPoint &pt);
    static QTreeWidgetItem *selectUp(QAction *action=nullptr,
                                     QMenu *parentMenu=nullptr,
                                     bool select=true);
    static QTreeWidgetItem *selectUp(const App::SubObjectT &objT,
                                     QMenu *parentMenu=nullptr,
                                     bool select=true);
    static void expandSelectedItems(TreeItemMode mode);
    static bool setupObjectMenu(QMenu &menu, const App::SubObjectT *sobj=nullptr);
    static void restoreDocumentItem(Gui::Document *gdoc, Base::XMLReader &reader);
    static bool saveDocumentItem(const Gui::Document *gdoc, Base::Writer &writer, const char *key);

    static int iconSize();

    int iconHeight() const;
    void setIconHeight(int height);

    int itemSpacing() const;
    void setItemSpacing(int);

    bool eventFilter(QObject *, QEvent *ev) override;

    struct SelInfo {
        App::DocumentObject *topParent;
        std::string subname;
        ViewProviderDocumentObject *parentVp;
        ViewProviderDocumentObject *vp;
    };
    /* Return a list of selected object of a give document and their parent
     *
     * This function can return the non-group parent of the selected object,
     * which Gui::Selection() cannot provide.
     */
    static std::vector<SelInfo> getSelection(App::Document *doc=0);

    static TreeWidget *instance();

    static const int DocumentType;
    static const int ObjectType;

    void markItem(const App::DocumentObject* Obj,bool mark);
    void syncView(ViewProviderDocumentObject *vp);

    virtual void selectAll() override;

    const char *getTreeName() const;

    static void updateStatus(bool delay=true);

    // Check if obj can be considered as a top level object
    static void checkTopParent(App::DocumentObject *&obj, std::string &subname);

    DocumentItem *getDocumentItem(const Gui::Document *) const;

    static Gui::Document *selectedDocument();

    void startDragging();

    void resetItemSearch();
    void startItemSearch(QLineEdit*);
    void itemSearch(const QString &text, bool select);

protected:
    void _selectAllInstances(const ViewProviderDocumentObject &vpd);
    void _selectLinkedObject(App::DocumentObject *linked);
    void _selectAllLinks(App::DocumentObject *obj);
    void _expandSelectedItems(TreeItemMode mode);
    bool _setupObjectMenu(DocumentObjectItem *item, QMenu &menu);
    void _setupDocumentMenu(DocumentItem *item, QMenu &menu);
    void _setupSelUpSubMenu(QMenu *parentMenu, DocumentItem *docitem,
            QTreeWidgetItem *item=nullptr, const App::SubObjectT *objT=nullptr);

    /// Observer message from the Selection
    void onSelectionChanged(const SelectionChanges& msg) override;
    void contextMenuEvent (QContextMenuEvent * e) override;
    /** @name Drag and drop */
    //@{
    void startDrag(Qt::DropActions supportedActions) override;
    bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data,
                      Qt::DropAction action) override;
    Qt::DropActions supportedDropActions () const override;
    QMimeData * mimeData (const QList<QTreeWidgetItem *> items) const override;
    void dragEnterEvent(QDragEnterEvent * event) override;
    void dragLeaveEvent(QDragLeaveEvent * event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    //@}
    bool event(QEvent *e) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;

protected:
    void showEvent(QShowEvent *) override;
    void hideEvent(QHideEvent *) override;
    void leaveEvent(QEvent *) override;
    void _updateStatus(bool delay=true);
    void _dragMoveEvent(QDragMoveEvent *event, bool *replace = nullptr);
    bool onDoubleClickItem(QTreeWidgetItem *);

protected Q_SLOTS:
    void onCreateGroup();
    void onRelabelObject();
    void onActivateDocument(QAction*);
    void onStartEditing();
    void onFinishEditing();
    void onSkipRecompute(bool on);
    void onAllowPartialRecompute(bool on);
    void onReloadDoc();
    void onCloseDoc();
    void onMarkRecompute();
    void onRecomputeObject();
    void onPreSelectTimer();
    void onSelectTimer();
    void onShowHidden();
    void onShowTempDoc();
    void onHideInTree();
    void onSearchObjects();
    void onToolTipTimer();

private Q_SLOTS:
    void onItemSelectionChanged(void);
    void onItemEntered(QTreeWidgetItem * item);
    void onItemCollapsed(QTreeWidgetItem * item);
    void onItemExpanded(QTreeWidgetItem * item);
    void onUpdateStatus(void);
    void onItemPressed();

Q_SIGNALS:
    void emitSearchObjects();

private:
    void slotNewDocument(const Gui::Document&, bool);
    void slotDeleteDocument(const Gui::Document&);
    void slotRenameDocument(const Gui::Document&);
    void slotActiveDocument(const Gui::Document&);
    void slotRelabelDocument(const Gui::Document&);
    void slotShowHidden(const Gui::Document &);
    void slotChangedViewObject(const Gui::ViewProvider &, const App::Property &);
    void slotChangedChildren(const Gui::ViewProviderDocumentObject &);
    void slotStartOpenDocument();
    void slotFinishOpenDocument();
    void _slotDeleteObject(const Gui::ViewProviderDocumentObject&, DocumentItem *deletingDoc);
    void slotDeleteObject(const Gui::ViewProviderDocumentObject&);
    void slotChangeObject(const Gui::ViewProviderDocumentObject&, const App::Property &prop);
    void slotTouchedObject(const App::DocumentObject&);

    void changeEvent(QEvent *e) override;
    void setupText();

private:
    class Private;
    std::unique_ptr<Private> pimpl;

    QAction* createGroupAction;
    QAction* relabelObjectAction;
    QAction* finishEditingAction;
    QAction* skipRecomputeAction;
    QAction* allowPartialRecomputeAction;
    QAction* markRecomputeAction;
    QAction* recomputeObjectAction;
    QAction* showHiddenAction;
    QAction* showTempDocAction;
    QAction* hideInTreeAction;
    QAction* reloadDocAction;
    QAction* closeDocAction;
    QAction* searchObjectsAction;
    static QTreeWidgetItem *contextItem;
    App::DocumentObject *searchObject;
    Gui::Document *searchDoc;
    Gui::Document *searchContextDoc;
    DocumentObjectItem *editingItem;
    DocumentObjectItem *hiddenItem;
    DocumentItem *currentDocItem;
    QTreeWidgetItem* rootItem;
    QTimer* statusTimer;
    QTimer* selectTimer;
    QTimer* preselectTimer;
    QTimer* toolTipTimer;
    QTime preselectTime;
    QPixmap documentPixmap;
    QPixmap documentPartialPixmap;
    QPixmap documentTempPixmap;
    std::unordered_map<const Gui::Document*,DocumentItem*> DocumentMap;
    std::unordered_map<App::DocumentObject*,std::set<DocumentObjectDataPtr> > ObjectTable;

    enum ChangedObjectStatus {
        CS_Error,
    };
    std::unordered_map<App::DocumentObject*,std::bitset<32> > ChangedObjects;

    std::unordered_map<std::string,std::vector<long> > NewObjects;

    static std::set<TreeWidget*> Instances;

    std::string myName; // for debugging purpose
    int updateBlocked = 0;

    friend class DocumentItem;
    friend class DocumentObjectItem;
    friend class TreeParams;
    friend class TreeWidgetItemDelegate;

    typedef boost::signals2::connection Connection;
    Connection connectNewDocument;
    Connection connectDelDocument;
    Connection connectRenDocument;
    Connection connectActDocument;
    Connection connectRelDocument;
    Connection connectShowHidden;
    Connection connectChangedViewObj;
    Connection connectChangedChildren;
    Connection connectFinishRestoreDocument;
};

class TreePanel : public QWidget
{
    Q_OBJECT

public:
    TreePanel(const char *name, QWidget* parent=nullptr);
    virtual ~TreePanel();

    bool eventFilter(QObject *obj, QEvent *ev);

private Q_SLOTS:
    void accept();
    void showEditor();
    void hideEditor();
    void itemSearch(const QString &text);

private:
    QLineEdit* searchBox;
    TreeWidget* treeWidget;
};

/**
 * The dock window containing the tree view.
 * @author Werner Mayer
 */
class TreeDockWidget : public Gui::DockWindow
{
    Q_OBJECT

public:
    TreeDockWidget(Gui::Document*  pcDocument,QWidget *parent=0);
    ~TreeDockWidget();
};


/** Helper class to read/write tree view options
 *
 * The parameters are stored under group "User parameter:BaseApp/Preferences/TreeView".
 * Call TreeParams::ParamName/setParamName() to get/set parameter.
 * To add a new parameter, add a new line under FC_TREEPARAM_DEFS using macro
 *
 * @code
 *      FC_TREEPARAM_DEF(parameter_name, c_type, parameter_type, default_value)
 * @endcode
 *
 * If there is special handling on parameter change, use FC_TREEPARAM_DEF2()
 * instead, and add a function with the following signature in Tree.cpp,
 *
 * @code
 *      void TreeParams:on<ParamName>Changed()
 * @endcode
 */
class GuiExport TreeParams : public ParameterGrp::ObserverType {
public:
    TreeParams();
    void OnChange(Base::Subject<const char*> &, const char* sReason);
    static TreeParams *Instance();

#define FC_TREEPARAM_DEFS \
    FC_TREEPARAM_DEF2(SyncSelection,bool,Bool,true) \
    FC_TREEPARAM_DEF(SyncView,bool,Bool,true) \
    FC_TREEPARAM_DEF(PreSelection,bool,Bool,true) \
    FC_TREEPARAM_DEF(SyncPlacement,bool,Bool,false) \
    FC_TREEPARAM_DEF(RecordSelection,bool,Bool,true) \
    FC_TREEPARAM_DEF2(DocumentMode,int,Int,2) \
    FC_TREEPARAM_DEF(StatusTimeout,int,Int,100) \
    FC_TREEPARAM_DEF(SelectionTimeout,int,Int,100) \
    FC_TREEPARAM_DEF(PreSelectionTimeout,int,Int,500) \
    FC_TREEPARAM_DEF(PreSelectionDelay,int,Int,700) \
    FC_TREEPARAM_DEF(PreSelectionMinDelay,int,Int,200) \
    FC_TREEPARAM_DEF(RecomputeOnDrop,bool,Bool,true) \
    FC_TREEPARAM_DEF(KeepRootOrder,bool,Bool,true) \
    FC_TREEPARAM_DEF(TreeActiveAutoExpand,bool,Bool,true) \
    FC_TREEPARAM_DEF(Indentation,int,Int,0) \
    FC_TREEPARAM_DEF2(ResizableColumn,bool,Bool,false) \
    FC_TREEPARAM_DEF(LabelExpression,bool,Bool,false) \
    FC_TREEPARAM_DEF2(IconSize,int,Int,0) \
    FC_TREEPARAM_DEF2(FontSize,int,Int,0) \
    FC_TREEPARAM_DEF2(ItemSpacing,int,Int,2) \

#undef FC_TREEPARAM_DEF
#define FC_TREEPARAM_DEF(_name,_type,_Type,_default) \
    static _type _name() {return Instance()->_##_name;} \
    static void set##_name(_type);\

#undef FC_TREEPARAM_DEF2
#define FC_TREEPARAM_DEF2(_name,_type,_Type,_default) \
    FC_TREEPARAM_DEF(_name,_type,_Type,_default) \
    void on##_name##Changed();\

    FC_TREEPARAM_DEFS

private:

#undef FC_TREEPARAM_DEF
#define FC_TREEPARAM_DEF(_name,_type,_Type,_default) \
    _type _##_name;

#undef FC_TREEPARAM_DEF2
#define FC_TREEPARAM_DEF2 FC_TREEPARAM_DEF

    FC_TREEPARAM_DEFS

    ParameterGrp::handle handle;
};

}

#endif // GUI_TREE_H
