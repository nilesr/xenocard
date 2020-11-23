class CardOverlay extends React.Component {
	render() {
		const style = {
			position: "absolute",
			width: "100%",
			height: "100%",
			display: "inline-block",
			borderRadius: "2.5% 3%", // could actually recalculate it from the scale but close enough
			color: "white",
		};
		let down_marker = null;
		if (this.props.down) {
			style.backgroundColor = "red";
			style.opacity = "0.45";
			style.zIndex = 9;
			down_marker = <div style={{
					position: "relative",
					top: "30%",
					width: "100%",
					textAlign: "center",
					fontSize: "200%",
				}}>
				DOWN
			</div>
		} else {
			style.backgroundColor = "lime";
			style.animation = "fade_opacity 1s infinite";
			style.animationDirection = "alternate";
			style.opacity = "0.25";
			style.zIndex = 10;
		}
		return <div style={style}>
			{down_marker}
		</div>;
	}
}

window.CardOverlay = CardOverlay;
