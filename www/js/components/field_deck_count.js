const FieldDeckCount = function(props) {
	return <div style={{
		position: "absolute",
		top: (100*props.y/3000) + "%",
		left: (100*props.x/3000) + "%",
		height: ((120/3000) * props.height),
		width: ((420/3000) * props.height),
		color: "yellow",
		textAlign: "center",
		fontSize: ((100/3000) * props.height) + "px",
	}}>
		{props.count}
	</div>;
};

window.FieldDeckCount = FieldDeckCount;
