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

        function test_undoMove()
        {

        }
    }
}

