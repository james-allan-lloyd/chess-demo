import QtQuick 2.0
import QtTest 1.0
import com.vividcode.Chess 1.0 as Chess

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


        function test_recordMoves()
        {
            var pawn = board.create("pawn", 0, 0)
            verify(recorder.move(pawn, 0, 1))
            recorder.save("file:test.chess")
            board.clearPieces()
            verify(recorder.load("file:test.chess"))
            var pawn = board.create("pawn", 0, 0)
            verify(recorder.canRedo, "Redo the pawn move is available")
            verify(recorder.redo(), "Move is redone")
            verify(!board.cell(0,0), "Pawn is not in created position")
            verify(board.cell(0,1), "Pawn is in moved position")
        }


        function test_recordTake()
        {
            var blackPawn = board.create("pawn", 0, 0, Chess.BoardModel.BLACK);
            var whitePawn = board.create("pawn", 1, 1, Chess.BoardModel.WHITE);

            verify(recorder.move(blackPawn, 1, 1), "Black pawn takes white pawn")
            recorder.save("file:test.chess")

            // reset the board
            board.clearPieces()
            var blackPawn = board.create("pawn", 0, 0, Chess.BoardModel.BLACK);
            var whitePawn = board.create("pawn", 1, 1, Chess.BoardModel.WHITE);

            verify(recorder.load("file:test.chess"))
            verify(recorder.redo(), "Move is redone")
            verify(!board.cell(0,0), "Black pawn has moved")
            compare(board.cell(1,1).color, Chess.BoardModel.BLACK, "White pawn is taken by black pawn after load")

            // Make sure the history entries have enough information to
            // actually undo moves, not just redo them
            verify(recorder.undo(), "Move is undone")
            verify(board.cell(1,1), "Taken piece is restored")
            compare(board.cell(0,0).color, Chess.BoardModel.BLACK, "Attacking piece is in original position")
            compare(board.cell(1,1).color, Chess.BoardModel.WHITE, "Taken piece is restored")
        }


        function test_recordPawnHasMoved()
        {
            var blackPawn = board.create("pawn", 0, 0, Chess.BoardModel.BLACK);
            verify(recorder.move(blackPawn, 0, 2))
            recorder.save("file:test.chess")

            board.clearPieces()
            board.create("pawn", 0, 0, Chess.BoardModel.BLACK);
            recorder.load("file:test.chess")
            verify(recorder.redo())  // first redo will work:w
            verify(recorder.undo())
            blackPawn = board.cell(0, 0)
            verify(blackPawn.isValidMove(Qt.point(0,2)), "Pawn should still be able to double move")
            verify(recorder.redo())
        }


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


        function test_multipleUndoRedo()
        {
            recorder.create("pawn", 0, 0)
            recorder.create("pawn", 0, 1)
            verify(recorder.undo())
            verify(recorder.undo())
            verify(!board.cell(0, 0))
            verify(!board.cell(0, 1))
            verify(recorder.redo())
            verify(board.cell(0, 0))
            verify(!board.cell(0, 1))
            verify(recorder.redo())
            verify(board.cell(0, 1))
            verify(board.cell(0, 0))

        }

        function test_noRedoUndoAfterRestart()
        {
            recorder.create("pawn", 0, 0)
            verify(recorder.undo())
            verify(recorder.redo())
            recorder.restart()
            verify(!recorder.canUndo, "Undo not available after restart")
            verify(!recorder.canRedo, "Redo not available after restart")
            verify(!recorder.undo(), "Undo not available after restart")
            verify(!recorder.redo(), "Redo not available after restart")
            verify(board.cell(Qt.point(0,0)))
        }

        function test_createsAreCompressed()
        {

        }


        function test_undoMove()
        {
            var pawn = recorder.create("pawn", 0, 0)
            verify(recorder.move(pawn, 0, 1))
            verify(!board.cell(0, 0))
            verify(board.cell(0, 1))
            verify(recorder.undo())
            verify(board.cell(0, 0))
            verify(!board.cell(0, 1))
        }

        function test_invalidMovesAreNotRecorded()
        {
            var pawn = board.create("pawn", 0, 0)
            verify(!recorder.move(pawn, 0, 120))
            verify(!recorder.undo())
            verify(!recorder.redo())
        }

        function test_undoTake()
        {
            var pawn = recorder.create("pawn", 0, 0)
            verify(recorder.move(pawn, 0, 1))

            var blackPawn = board.create("pawn", 0, 0, Chess.BoardModel.BLACK);
            var whitePawn = board.create("pawn", 1, 1, Chess.BoardModel.WHITE);

            verify(recorder.move(blackPawn, 1, 1))
            verify(recorder.undo())
            verify(board.cell(1,1))
            compare(board.cell(0,0).color, Chess.BoardModel.BLACK, "Attacking piece is in original position")
            compare(board.cell(1,1).color, Chess.BoardModel.WHITE, "Taken piece is restored")

            recorder.restart()

            whitePawn = board.cell(1, 1)
            verify(recorder.move(whitePawn, 0, 0))
            verify(recorder.undo())
            verify(board.cell(1,1))
            compare(board.cell(0,0).color, Chess.BoardModel.BLACK, "Attacking piece is in original position")
            compare(board.cell(1,1).color, Chess.BoardModel.WHITE, "Taken piece is restored")
        }


        function test_redoTake()
        {
            var blackPawn = board.create("pawn", 0, 0, Chess.BoardModel.BLACK);
            var whitePawn = board.create("pawn", 1, 1, Chess.BoardModel.WHITE);

            verify(recorder.move(blackPawn, 1, 1), "White pawn is taken")
            verify(recorder.undo())
            verify(recorder.redo(), "Move is redone")
            verify(!board.cell(0, 0), "Black pawn has moved")
            compare(board.cell(1,1).color, Chess.BoardModel.BLACK, "Black now occupies (1,1)")
        }


        function printMoves(piece)
        {
            console.log("Valid moves for " + piece.objectName)
            for(var y=0; y<8; ++y)
            {
                for(var x=0; x<8; ++x)
                {
                    if(piece.isValidMove(x,y))
                    {
                        console.log(x, y)
                    }
                }
            }
        }


        function test_undoPawnMoveRetainsMovedStatus()
        {
            var whitePawn = board.create("pawn", 0, 7, Chess.BoardModel.WHITE);
            verify(recorder.move(whitePawn, 0, 6))
            verify(recorder.move(whitePawn, 0, 5))
            verify(recorder.undo())
            whitePawn = board.cell(0, 6)
            verify(whitePawn)
            verify(whitePawn.isValidMove(0, 5))
            verify(!whitePawn.isValidMove(0, 4))
        }

        function test_undoPawnTakeRetainsMovedStatus()
        {
            var whitePawn = board.create("pawn", 0, 6, Chess.BoardModel.WHITE);
            var blackPawn = board.create("pawn", 1, 4, Chess.BoardModel.BLACK);
            verify(recorder.move(blackPawn, 1, 5), "Black has moved from 1,5 to 1,6")
            verify(recorder.move(whitePawn, 1, 5), "White takes black")
            verify(recorder.undo())

            whitePawn = board.cell(0,6)
            blackPawn = board.cell(1,5)

            compare(whitePawn.color, Chess.BoardModel.WHITE)
            compare(blackPawn.color, Chess.BoardModel.BLACK)
            verify(blackPawn.isValidMove(1, 6))
            verify(!blackPawn.isValidMove(1, 7))
        }

        function test_movingClearsHistory()
        {
            var whitePawn = board.create("pawn", 0, 6, Chess.BoardModel.WHITE);
            verify(recorder.move(whitePawn, 0, 5))
            verify(recorder.move(whitePawn, 0, 4))
            verify(recorder.undo())
            // FIXME: don't delete pointers on remove
            whitePawn = board.cell(0, 5)
            verify(whitePawn)
            verify(recorder.move(whitePawn, 0, 4))
            verify(!recorder.canRedo)
            verify(!recorder.redo())
        }
    }
}

