const FieldCard = function(props) {
	return <div style={{
		position: "absolute",
		top: (100*props.y/3000) + "%",
		left: (100*props.x/3000) + "%",
	}}>
		<Card
			id={props.id}
			card={props.card}
			scale={((300/3000) * props.height)/500}
			onClick={props.onClick}
			selected={props.selected}
			displayHealth={props.displayHealth} />
	</div>;
};

window.FieldCard = FieldCard;
