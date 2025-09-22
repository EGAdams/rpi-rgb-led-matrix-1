# Persona
World-class iOS Developer

# Your Task 1
Please analyze the following Swift source code.  Pay particular attention to how it is not using constraints properly.  It does not consistantly show the same on all device sizes.

# Source code to analyze
```swift
// Activate constraints
NSLayoutConstraint.activate([
    // Position the "Finish" button at the bottom-right, with a standard margin
    self.the_finish_button.bottomAnchor.constraint( equalTo: self.view.safeAreaLayoutGuide.bottomAnchor, constant: -20 ),
    self.the_finish_button.trailingAnchor.constraint( equalTo: self.view.safeAreaLayoutGuide.trailingAnchor, constant: -20 ),
    self.the_finish_button.widthAnchor.constraint( equalToConstant: 100 ), // A fixed width for the "Finish" button
    self.the_finish_button.heightAnchor.constraint( equalToConstant: 50 ), // A fixed height for the "Finish" button

    // Position the "Next Run" button to the left of the "Finish" button, with a standard space between them
    self.nextRunButton.bottomAnchor.constraint( equalTo: self.view.safeAreaLayoutGuide.bottomAnchor, constant: -20 ),
    self.nextRunButton.trailingAnchor.constraint( equalTo: self.the_finish_button.leadingAnchor, constant: -10 ), // spc betw
    self.nextRunButton.widthAnchor.constraint( equalToConstant: 100 ), // A fixed width for the "Next Run" button
    self.nextRunButton.heightAnchor.constraint( equalToConstant: 50 ), // A fixed height for the "Next Run" button

    // Update Instruction label constraints to align the left edge with the "Next Run" button
    self.instruction.bottomAnchor.constraint( equalTo: self.nextRunButton.topAnchor, constant: -20 ),
    self.instruction.leadingAnchor.constraint( equalTo: self.nextRunButton.leadingAnchor),
    self.instruction.trailingAnchor.constraint( equalTo: self.view.safeAreaLayoutGuide.trailingAnchor, constant: -20 ),
    self.instruction.heightAnchor.constraint( equalToConstant: 100 ),

    // Update currentAirportName label constraints to align the left edge with the instruction lable above
    self.currentAirportName.bottomAnchor.constraint( equalTo: self.instruction.topAnchor, constant: -20 ),
    self.currentAirportName.leadingAnchor.constraint( equalTo: self.instruction.leadingAnchor),
    self.currentAirportName.trailingAnchor.constraint( equalTo: self.instruction.trailingAnchor),
    self.currentAirportName.heightAnchor.constraint( equalToConstant: 100 ),

    // Update the program_name label constraints to align the left edge with the currentAirportName label below
    self.program_name.bottomAnchor.constraint( equalTo: self.currentAirportName.topAnchor, constant: 40 ),
    self.program_name.leadingAnchor.constraint( equalTo: self.currentAirportName.leadingAnchor),
    self.program_name.trailingAnchor.constraint( equalTo: self.currentAirportName.trailingAnchor),
    self.program_name.heightAnchor.constraint( equalToConstant: 100 ),
    
    // Force_label constraints
    self.force_label.topAnchor.constraint( equalTo: self.instruction.bottomAnchor, constant: 20 ),
    self.force_label.leadingAnchor.constraint( equalTo: self.view.safeAreaLayoutGuide.leadingAnchor, constant: 20 ),

    // Number_of_tests constraints
    self.number_of_tests.topAnchor.constraint( equalTo: self.force_label.bottomAnchor, constant: 20 ),
    self.number_of_tests.leadingAnchor.constraint( equalTo: self.force_label.leadingAnchor ),

    self.gravityLabel.centerXAnchor.constraint(equalTo: self.speedScaleView.centerXAnchor),
    self.gravityLabel.centerYAnchor.constraint(equalTo: self.speedScaleView.centerYAnchor, constant: 80 ), // Move down by 80 points
    self.gravityLabel.widthAnchor.constraint(equalToConstant: CGFloat( labelWidth)),
    self.gravityLabel.heightAnchor.constraint(equalToConstant: CGFloat( labelHeight)),

    self.mphLabel.centerXAnchor.constraint(equalTo: self.speedScaleView.centerXAnchor),
    self.mphLabel.topAnchor.constraint(equalTo: self.gravityLabel.bottomAnchor, constant: 5 ), // 10 below gravityLabel
    self.mphLabel.widthAnchor.constraint(equalToConstant: CGFloat( labelWidth )),
    self.mphLabel.heightAnchor.constraint(equalToConstant: CGFloat( labelHeight ))
])
```

# Your Task 2
Rewrite the force_label constraint so that the force label sits right on top of the "Next Run" button.
Rewrite the number_of_tests constraint so that it sits right on top of the "Finish" button.