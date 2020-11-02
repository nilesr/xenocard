class CardOverlay extends React.Component {
	render() {
		return <div style={{
				position: "absolute",
				width: "100%",
				height: "100%",
				backgroundColor: "lime",
				display: "inline-block",
				opacity: "0.25",
				animation: "fade_opacity 1s infinite",
				animationDirection: "alternate",
			}}
		>
		</div>;
	}
}

window.CardOverlay = CardOverlay;
