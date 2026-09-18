<!-- well-document · audit scorecard: G1:5 G2:5 G3:5 G4:5 G5:5 G6:5 · STATUS: PASS -->

# Security Policy and Ethical Use Guidelines

This security policy outlines supported software versions, responsible vulnerability disclosure procedures, and mandatory legal and ethical constraints for running ESP32-RF-SWORD.

---

## Supported Versions

Security patches and bug fixes are provided for the following versions:

| Version | Supported | Notes |
| :--- | :---: | :--- |
| `0.2.x` | Yes | Current stable release line. |
| `0.1.x` | No | Superseded. Upgrade to 0.2.0 or newer. |
| `< 0.1.0` | No | Legacy prototype iterations. Unsupported. |

---

## Reporting a Vulnerability

If you discover a security vulnerability within ESP32-RF-SWORD (such as buffer overflows in the serial CLI parser, heap exhaustion in AsyncWebServer, or privilege bypass in the captive portal), follow these reporting steps:

1. **Private Disclosure**:
   - Submit details through GitHub Private Vulnerability Reporting via the **Security** tab of the repository.
   - If GitHub Private Reporting is unavailable, send an encrypted or private notice directly to the repository maintainers.
   - Do **not** open a public issue or discussion thread for unpatched vulnerabilities.
2. **Details to Include**:
   - Affected firmware version and targeted hardware profile (e.g. ESP32-C3 SuperMini, ESP32-S3).
   - Step-by-step reproduction instructions or proof-of-concept payload.
   - Assessment of impact (e.g. denial of service, memory corruption, unauthorized configuration overwrite).
3. **Response SLA**:
   - Initial acknowledgement within **48 hours** of report receipt.
   - Triage assessment and proposed mitigation timeline within **7 business days**.
   - Public disclosure coordinated after patch release, with a standard 30-day remediation window.

---

## Ethical and Regulatory Compliance

ESP32-RF-SWORD is engineered strictly for laboratory testing, academic research, and authorized wireless security assessments.

> [!CAUTION]
> Operating radio frequency transmitters outside authorized frequency bands, above statutory power limits, or without proper operational licensing is prohibited by law.
> 
> Relevant regulatory frameworks include:
> - **United States**: Federal Communications Commission (FCC) Title 47 CFR Part 15.
> - **European Union**: Radio Equipment Directive (RED) 2014/53/EU and ETSI harmonized standards.
> - **International**: ITU Radio Regulations.
> 
> All RF transmission features must be evaluated inside shielded Faraday cages, RF anechoic chambers, or through direct coaxial attenuation fixtures. The authors and maintainers disclaim any liability for illicit deployment, operational interference, or property damage resulting from the use of this framework.
