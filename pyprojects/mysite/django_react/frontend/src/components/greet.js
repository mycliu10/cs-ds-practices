import React, { Component } from "react";
import { render } from "react-dom";

function Greet(props) {
    const name = "Dear User"
    return (
    <div>
        <h1>Hello {name}!</h1>
    </div>
    )
}
export default Greet


const greetDiv = document.getElementById("greet");
render(<Greet />, greetDiv);


