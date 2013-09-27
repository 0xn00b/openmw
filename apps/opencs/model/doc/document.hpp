#ifndef CSM_DOC_DOCUMENT_H
#define CSM_DOC_DOCUMENT_H

#include <string>

#include <boost/filesystem/path.hpp>

#include <QUndoStack>
#include <QObject>
#include <QTimer>

#include "../world/data.hpp"

#include "../tools/tools.hpp"

#include "state.hpp"
#include "saving.hpp"

class QAbstractItemModel;

namespace ESM
{
    struct GameSetting;
    struct Global;
}

namespace CSMDoc
{
    class Document : public QObject
    {
            Q_OBJECT

        private:

            boost::filesystem::path mSavePath;
            std::vector<boost::filesystem::path> mContentFiles;
            CSMWorld::Data mData;
            CSMTools::Tools mTools;
            Saving mSaving;

            // It is important that the undo stack is declared last, because on desctruction it fires a signal, that is connected to a slot, that is
            // using other member variables.  Unfortunately this connection is cut only in the QObject destructor, which is way too late.
            QUndoStack mUndoStack;

            // not implemented
            Document (const Document&);
            Document& operator= (const Document&);

            void load (const std::vector<boost::filesystem::path>::const_iterator& begin,
                const std::vector<boost::filesystem::path>::const_iterator& end, bool lastAsModified);
            ///< \param lastAsModified Store the last file in Modified instead of merging it into Base.

            void createBase();

            void addGmsts();

            void addOptionalGmsts();

            void addOptionalGlobals();

            void addOptionalGmst (const ESM::GameSetting& gmst);

            void addOptionalGlobal (const ESM::Global& global);

        public:

            Document (const std::vector<boost::filesystem::path>& files,
                const boost::filesystem::path& savePath, bool new_,
                const boost::filesystem::path& projectPath);
            ///< \param projectPath Location of file that can be used to store additional data for
            /// this project.

            ~Document();

            QUndoStack& getUndoStack();

            int getState() const;

            const boost::filesystem::path& getSavePath() const;

            const std::vector<boost::filesystem::path>& getContentFiles() const;
            ///< \attention The last element in this collection is the file that is being edited,
            /// but with its original path instead of the save path.

            void save();

            CSMWorld::UniversalId verify();

            void abortOperation (int type);

            const CSMWorld::Data& getData() const;

            CSMWorld::Data& getData();

            CSMTools::ReportModel *getReport (const CSMWorld::UniversalId& id);
            ///< The ownership of the returned report is not transferred.

        signals:

            void stateChanged (int state, CSMDoc::Document *document);

            void progress (int current, int max, int type, int threads, CSMDoc::Document *document);

        private slots:

            void modificationStateChanged (bool clean);

            void operationDone (int type);

        public slots:

            void progress (int current, int max, int type);
    };
}

#endif
