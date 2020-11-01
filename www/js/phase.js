function nextPhase(phase) {
	switch (phase) {
		case "P1_SHUFFLE": return "P2_SHUFFLE";
		case "P2_SHUFFLE": return "P1_DRAW";
		case "P1_DRAW": return "P1_MOVE";
		case "P1_MOVE": return "P1_EVENT";
		case "P1_EVENT": return "P1_SET";
		case "P1_SET": return "P1_BLOCK";
		case "P1_BLOCK": return "P1_BATTLE";
		case "P1_BATTLE": return "P1_ADJUST";
		case "P1_ADJUST": return "P2_DRAW";
		case "P2_DRAW": return "P2_MOVE";
		case "P2_MOVE": return "P2_EVENT";
		case "P2_EVENT": return "P2_SET";
		case "P2_SET": return "P2_BLOCK";
		case "P2_BLOCK": return "P2_BATTLE";
		case "P2_BATTLE": return "P2_ADJUST";
		case "P2_ADJUST": return "P1_DRAW";
	}
	throw "Invalid phase for nextPhase";
}

function playerSideForPhase(phase) {
	switch (phase) {
		case "P1_SHUFFLE": return "P1";
		case "P2_SHUFFLE": return "P2";

		case "P1_DRAW": return "P1";
		case "P1_MOVE": return "P1";
		case "P1_EVENT": return "P1";
		case "P1_SET": return "P1";
		case "P1_BLOCK": return "P2";
		case "P1_BATTLE": return "P1";
		case "P1_ADJUST": return "P1";
		case "P2_DRAW": return "P2";
		case "P2_MOVE": return "P2";
		case "P2_EVENT": return "P2";
		case "P2_SET": return "P2";
		case "P2_BLOCK": return "P1";
		case "P2_BATTLE": return "P2";
		case "P2_ADJUST": return "P2";
	}
	throw "Invalid phase for playerSideForPhase";
}

window.nextPhase = nextPhase;
window.playerSideForPhase = playerSideForPhase;
