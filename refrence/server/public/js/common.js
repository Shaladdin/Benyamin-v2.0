function removeAllEvents(node) {
    let newNode = node.cloneNode(true);
    node.parentNode.replaceChild(newNode, node);
    return newNode;
}

const { stringify, parse: jsonify } = JSON;

// turn string into json SAFELY 
function objify(str) {
    try {
        return jsonify(String(str));
    } catch (e) {
        console.warn(e, str);
        return false;
    }
}
