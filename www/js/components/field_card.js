const FieldCard = function(props) {
	return <div style={{
		position: "absolute",
		top: (100*props.y/3000) + "%",
		left: (100*props.x/3000) + "%",
	}}>
		<Card
			card={props.card || {image_location: {set: 1, top: 0, left: 0}}}
			scale={((300/3000) * props.height)/500}
			onClick={props.onClick} />
	</div>;
};

window.FieldCard = FieldCard;
