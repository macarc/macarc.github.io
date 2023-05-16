let target = "";
let numDigits = 2;
let hexToDecimal = false;

// Pull the new number of digits from the UI
// and update the UI if necessary
function updateNumberOfDigits() {
  const newNumDigits = parseInt(document.querySelector("#number-of-digits").value);
  if (numDigits !== newNumDigits) {
    numDigits = newNumDigits;
    updateTarget();
  }
}

// Pull the new mode (hex->dec or dec->hex) from the UI
// and update the UI if necessary
function updateMode() {
  if (document.querySelector("#hex-to-dec").checked) {
    hexToDecimal = true;
  } else {
    hexToDecimal = false;
  }
  updateTarget();
}

// Getter for the text box that the user types into
function textBox() {
  return document.querySelector("#user-input");
}

// Returns true if the value in the text box is the
// correct conversion
function userIsCorrect() {
  if (hexToDecimal) {
    return parseInt(textBox().value) === target;
  } else {
    return parseInt(textBox().value, 16) === target;
  }
}

// Update the target number and the UI
function updateTarget() {
  const min = Math.pow(16, numDigits - 1);
  const max = Math.pow(16, numDigits);
  const oldTarget = target;
  while (target === oldTarget) {
    target = min + Math.floor(Math.random() * (max - min));
  }

  textBox().style.border = '';
  textBox().value = '';

  if (hexToDecimal) {
    document.querySelector("#target").innerText = "0x" + target.toString(16);
    document.querySelector("#pre-input").innerText = "";
    textBox().placeholder = "Type decimal..."
  } else {
    document.querySelector("#target").innerText = target.toString();
    document.querySelector("#pre-input").innerText = "0x";
    textBox().placeholder = "Type hex..."
  }
}

// Updates the UI to let the user know they got it wrong
function signalError() {
  textBox().style.border = '2px solid red';
}

document.addEventListener("DOMContentLoaded", () => {
  updateNumberOfDigits();
  updateMode();
  updateTarget();
  textBox().focus();


  document.querySelector("form").addEventListener("submit", e => {
    e.preventDefault();
    if (userIsCorrect()) {
      updateTarget();
    } else {
      signalError();
    }
  })

  document.querySelector("#number-of-digits").addEventListener("input", updateNumberOfDigits);
  document.querySelector("#hex-to-dec").addEventListener("input", updateMode);
  document.querySelector("#dec-to-hex").addEventListener("input", updateMode);
});
