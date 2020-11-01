class App extends React.Component {
	constructor(props) {
		super(props);
		const ws = new WebSocket('ws://localhost:8080/');
		ws.onclose = () => alert('Disconnected');
		ws.onmessage = (event) => this.onMessage(JSON.parse(event.data));
		this.state = {
			ws: ws,
			log: [],
		};
	}
	render() {
		return <ul>
			{this.state.log.map((m, i) => 
				<li key={i}>{JSON.stringify(m)}</li>
			)}
		</ul>;
	}
	onMessage(data) {
		this.setState(s => {return {log: s.log.concat([data])}; });
	}
};

window.App = App;

