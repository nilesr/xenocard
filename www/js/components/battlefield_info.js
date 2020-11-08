const BattlefieldInfo = function(props) {
	if (!props.card) return null;
	return <div style={{
		position: "absolute",
		top: (100*props.y/3000) + "%",
		left: (100*props.x/3000) + "%",
		width: (420/3000) * props.height + "px",
		height: (300/3000) * props.height + "px",
		backgroundColor: "black",
		// note to self, do not add padding
		borderRadius: (15/3000)*props.height + "px",
	}}>
		{/* TODO: HP bar with count on the right
			TODO: ATK stat on the right
			TODO: Battle card type image in the bottom left
			TODO: Whatever those blue dots are for */}
		{props.card.health}/{props.card.max_health}
		<br />
		{props.card.attack_pattern}
	</div>;
};

window.BattlefieldInfo = BattlefieldInfo;

