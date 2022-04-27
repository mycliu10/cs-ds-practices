import React, { Component } from "react";
import { render } from "react-dom";

export default class App extends Component {
    constructor(props) {
        super(props);
        this.state = {
            data: [],
            loaded: false,
            placeholder: "Loading data...",
            search_keyword: "banana"
        };
    }

//    componentDidMount() {
    getSearchKeyword(e) {
        this.setState({
            search_keyword: e.target.value
        });
    }

    handleButtonClicked() {
        fetch("/api/grocery" + "?keyword=" + this.state.search_keyword)
        .then( response => {
            if( response.status > 400 ) {
                return this.setState(() => {
                    return { placeholder: "Something went wrong!" };
                });
            }
            return response.json();
        })
        .then( data => {
            this.setState(() => {
                return {
                    data: data,
                    loaded: true,
                    placeholder: "Loaded."
                };
            });
        });
    }

    render() {
        return (
        <div>
            <div>
                <label>
                    search keyword: 
                </label>
                <br/>
                <input 
                    type="text" 
                    value={this.state.search_keyword}
                    onChange={this.getSearchKeyword.bind(this)}
                />
                <hr/>
                <button onClick={this.handleButtonClicked.bind(this)}>
                    Submit
                </button>
            </div>
                        
            <p>{this.state.placeholder}</p>
            {Object.entries(this.state.data).map(([key,value]) => (
                    <div>{key}:
                        {value.map((i) => (
                            <li>{i.name} :${i.currentPrice}</li>
                        ))}
                    </div>
            ))}
        </div>
        )
    }
}


const appDiv = document.getElementById("app");
render(<App />, appDiv);


