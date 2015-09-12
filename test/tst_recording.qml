import QtQuick 2.0
import QtTest 1.0
import com.luxoft.Chess 1.0 as Chess

Rectangle {

    TestCase {

        id: root
        name: "RecordingTests"

        Chess.BoardModel {
            id: board
        }

        Chess.Recorder {
            id: recorder
            model: board
        }

        function init()
        {
            board.clearPieces()
            recorder.restart()
            recorder.model = board
        }

        // function test_recordCreation()
        // {
        //     board.create("pawn", 0, 0)
        //     board.create("queen", 7, 7)
        //     board.save("test.chess")
        //     board.clearPieces()
        //     board.load("test.chess")
        //     verify(board.cell(Qt.point(0,0)) !== null, "Pawn is recreated")
        //     verify(board.cell(Qt.point(7,7)) !== null, "Queen is recreated")
        //     compare(board.cell(Qt.point(0,0)).objectName, "Pawn");
        //     compare(board.cell(Qt.point(7,7)).objectName, "Queen");
        // }


        // function test_recordMoves()
        // {
        //     var pawn = board.create("pawn", 0, 0)
        //     verify(pawn.moveTo(Qt.point(0, 1)))
        //     board.save("test.chess")
        //     board.clearPieces()
        //     board.load("test.chess")
        //     verify(board.cell(Qt.point(0,0)) === null, "Pawn is not in created position")
        //     verify(board.cell(Qt.point(0,1)) !== null, "Pawn is in moved position")
        // }


        function test_createWithoutModel()
        {
            recorder.model = null
            var pawn = recorder.create("pawn", 0, 0)
            verify(!pawn);
        }

        function test_undoCreate()
        {
            var pawn = recorder.create("pawn", 0, 0)
            compare(pawn, board.cell(Qt.point(0,0)), "Recorder creates pieces in the model")

            verify(recorder.undo(), "Undo was successful")
            verify(!board.cell(Qt.point(0,0)), "The creation was undone, no piece in the given cell")
        }

        function test_invalidCreatesAreNotRecorded()
        {
            var pawn = recorder.create("pawn", 12, 120)
            verify(!pawn, "Pawn was not created")
            verify(!recorder.undo())
        }

        function test_redoCreate()
        {
            var pawn = recorder.create("pawn", 0, 0, "white")
            verify(recorder.undo())
            verify(recorder.redo(), "Redo was successful")
            verify(board.cell(Qt.point(0,0)), "Piece exists")
            compare(board.cell(Qt.point(0,0)).color, Chess.BoardModel.WHITE, "Piece is correct color")
        }


        function test_actionsNotRecordedUnlessAtEndOfHistory()
        {

        }

        function test_multipleUndoRedo()
        {

        }

        function test_noRedoUndoAfterRestart()
        {
            recorder.create("pawn", 0, 0)
            verify(recorder.undo())
            verify(recorder.redo())
            recorder.restart()
            verify(!recorder.undo(), "Undo not available after restart")
            verify(!recorder.redo(), "Redo not available after restart")
            verify(board.cell(Qt.point(0,0)))
        }

        function test_createsAreCompressed()
        {

        }
    }
}
